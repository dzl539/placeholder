/**
 * LRU implementation
 */
#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace scudb {

    template <typename T> LRUReplacer<T>::LRUReplacer() {
        head = make_shared<Node>();
        tail = make_shared<Node>();
        head->next = tail;
        tail->prev = head;
    }

    template <typename T> LRUReplacer<T>::~LRUReplacer() {}

/*
 * Insert value into LRU
 */
    template <typename T> void LRUReplacer<T>::Insert(const T &value) {
        lock_guard<mutex> lck(latch);
        shared_ptr<Node> curent;
        if (map.find(value) != map.end()) {
            curent = map[value];
            shared_ptr<Node> prev = curent->prev;
            shared_ptr<Node> succ = curent->next;
            prev->next = succ;
            succ->prev = prev;
        } else {
            curent = make_shared<Node>(value);
        }
        shared_ptr<Node> temfir = head->next;
        curent->next = temfir;
        temfir->prev = curent;
        curent->prev = head;
        head->next = curent;
        map[value] = curent;
        return;
    }

/* If LRU is non-empty, pop the head member from LRU to argument "value", and
 * return true. If LRU is empty, return false
 */
    template <typename T> bool LRUReplacer<T>::Victim(T &value) {
        lock_guard<mutex> lck(latch);
        if (map.empty()==true) {
            return false;
        }
        shared_ptr<Node> temlast = tail->prev;
        tail->prev = temlast->prev;
        temlast->prev->next = tail;
        value = temlast->val;
        map.erase(temlast->val);
        return true;
    }

/*
 * Remove value from LRU. If removal is successful, return true, otherwise
 * return false
 */
    template <typename T> bool LRUReplacer<T>::Erase(const T &value) {
        lock_guard<mutex> lck(latch);
        if (map.find(value) != map.end()) {
            shared_ptr<Node> curent = map[value];
            curent->prev->next = curent->next;
            curent->next->prev = curent->prev;
        }
        return map.erase(value);
    }

    template <typename T> size_t LRUReplacer<T>::Size() {
        lock_guard<mutex> lck(latch);
        return map.size();
    }

    template class LRUReplacer<Page *>;
// test only
    template class LRUReplacer<int>;

} // namespace cmudb
