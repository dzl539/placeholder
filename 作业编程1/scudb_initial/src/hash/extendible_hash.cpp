#include <list>

#include "hash/extendible_hash.h"
#include "page/page.h"
#include <math.h>
using namespace std;

namespace scudb {

/*
 * constructor
 * array_size: fixed array size for each bucket
 */
    template <typename K, typename V>
    ExtendibleHash<K, V>::ExtendibleHash(size_t size) :  globalDepth(0),bucketSize(size),bucketNum(1) {
        buckets.push_back(make_shared<Bucket>(0));
    }
    template<typename K, typename V>
    ExtendibleHash<K, V>::ExtendibleHash() {
        ExtendibleHash(64);
    }

/*
 * helper function to calculate the hashing address of input key
 */
    template <typename K, typename V>
    size_t ExtendibleHash<K, V>::HashKey(const K &key) const{
        return hash<K>{}(key);
    }

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
    template <typename K, typename V>
    int ExtendibleHash<K, V>::GetGlobalDepth() const{
        lock_guard<mutex> lock(latch);
        return globalDepth;
    }

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
 */
    template <typename K, typename V>
    int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const {
        //lock_guard<mutex> lck2(latch);
        if (buckets[bucket_id]) {
            lock_guard<mutex> lck(buckets[bucket_id]->latch);
            if (buckets[bucket_id]->kmap.size() == 0) return -1;
            return buckets[bucket_id]->localDepth;
        }
        return -1;
    }

/*
 * helper function to return current number of bucket in hash table
 */
    template <typename K, typename V>
    int ExtendibleHash<K, V>::GetNumBuckets() const{
        lock_guard<mutex> lock(latch);
        return bucketNum;
    }

/*
 * lookup function to find value associate with input key
 */
    template <typename K, typename V>
    bool ExtendibleHash<K, V>::Find(const K &key, V &value) {

        int indexofkey = getIdx(key);
        lock_guard<mutex> lck(buckets[indexofkey]->latch);
        if (buckets[indexofkey]->kmap.find(key) != buckets[indexofkey]->kmap.end())
        {
            value = buckets[indexofkey]->kmap[key];
            return true;

        }
        else
        {
            return false;
        }

    }

    template <typename K, typename V>
    int ExtendibleHash<K, V>::getIdx(const K &key) const{
        lock_guard<mutex> lck(latch);
        return HashKey(key) & ((1<<globalDepth) - 1);
    }

/*
 * delete <key,value> entry in hash table
 * Shrink & Combination is not required for this project
 */
    template <typename K, typename V>
    bool ExtendibleHash<K, V>::Remove(const K &key) {
        int suoyin = getIdx(key);
        lock_guard<mutex> lck(buckets[suoyin]->latch);
        shared_ptr<Bucket> curent = buckets[suoyin];
        if (curent->kmap.find(key) == curent->kmap.end())
        {
            return false;
        }
        else
        {
            curent->kmap.erase(key);
            return true;
        }
    }

/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
    template <typename K, typename V>
    void ExtendibleHash<K, V>::Insert(const K &key, const V &value)
    {
        int indexofkey = getIdx(key);
        shared_ptr<Bucket> current = buckets[indexofkey];
        while (true) {
            lock_guard<mutex> lck(current->latch);
            if (current->kmap.find(key) != current->kmap.end() || current->kmap.size() < bucketSize)
            {
                current->kmap[key] = value;
                break;
            }
            int yanma = (pow(2,(current->localDepth)));
            current->localDepth++;

            {
                lock_guard<mutex> lck2(latch);
                if (current->localDepth > globalDepth)
                {

                    size_t length = buckets.size();
                    for (size_t ecph = 0; ecph < length; ecph++)
                    {
                        buckets.push_back(buckets[ecph]);
                    }
                    globalDepth = globalDepth + 1;

                }
                bucketNum = bucketNum + 1;
                auto newBucket = make_shared<Bucket>(current->localDepth);

                typename map<K, V>::iterator itmachine;
                for (itmachine = current->kmap.begin(); itmachine != current->kmap.end(); )
                {
                    if (HashKey(itmachine->first) & yanma)
                    {
                        newBucket->kmap[itmachine->first] = itmachine->second;
                        itmachine = current->kmap.erase(itmachine);
                    }
                    else itmachine++;
                }
                for (size_t ecph = 0; ecph <= buckets.size()-1; ecph++)
                {
                    if (buckets[ecph] == current && (ecph & yanma))
                        buckets[ecph] = newBucket;
                }
            }
            indexofkey = getIdx(key);
            current = buckets[indexofkey];
        }
    }



    template class ExtendibleHash<page_id_t, Page *>;
    template class ExtendibleHash<Page *, std::list<Page *>::iterator>;
// test purpose
    template class ExtendibleHash<int, std::string>;
    template class ExtendibleHash<int, std::list<int>::iterator>;
    template class ExtendibleHash<int, int>;
} // namespace cmudb
