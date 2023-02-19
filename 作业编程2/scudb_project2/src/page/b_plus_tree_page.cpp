/**
 * b_plus_tree_page.cpp
 */
#include "page/b_plus_tree_page.h"

namespace scudb {

/*
 * Helper methods to get/set page type
 * Page type enum class is defined in b_plus_tree_page.h
 */
bool BPlusTreePage::IsLeafPage() const { return page_type_ == IndexPageType::LEAF_PAGE; }

bool BPlusTreePage::IsRootPage() const { return parent_page_id_ == INVALID_PAGE_ID; }

void BPlusTreePage::SetPageType(IndexPageType page_type) { page_type_ = page_type; }

/*
 * Helper methods to get/set size (number of key/value pairs stored in that
 * page)
 */
int BPlusTreePage::GetSize() const { return size_; }

void BPlusTreePage::SetSize(int size) { size_ = size; }

void BPlusTreePage::IncreaseSize(int amount) { size_ += amount; }

/*
 * Helper methods to get/set max size (capacity) of the page
 */
int BPlusTreePage::GetMaxSize() const { return max_size_; }

void BPlusTreePage::SetMaxSize(int size) { max_size_ = size; }

/*
 * Helper method to get min page size
 * Generally, min page size == max page size / 2
 * With n = 4 in our example B+-tree, each leaf must contain at least 2 values, and at most 3 values.
 */
int BPlusTreePage::GetMinSize() const {
    bool judge1 = IsRootPage();
    bool judge2 = IsLeafPage();
  if (judge1 == true)
  {//why not 0 is min? because 1 is only have a pointer without key, so just equal to empty
    if(judge2 == true)
    {
        return 1;
    }
    else
    {
        return 2;
    }
//      return IsLeafPage() ? 1: 2;//root page & leaf page may be empty tree, so is 1; otherwise at least 1 node in root, size 2
  }
  return (max_size_ ) / 2;
}

/*
 * Helper methods to get/set parent page id
 */
page_id_t BPlusTreePage::GetParentPageId() const { return parent_page_id_; }

void BPlusTreePage::SetParentPageId(page_id_t parent_page_id) { parent_page_id_ = parent_page_id; }

/*
 * Helper methods to get/set self page id
 */
page_id_t BPlusTreePage::GetPageId() const { return page_id_; }

void BPlusTreePage::SetPageId(page_id_t page_id) { page_id_ = page_id; }

/*
 * Helper methods to set lsn
 */
void BPlusTreePage::SetLSN(lsn_t lsn) { lsn_ = lsn; }

/* for concurrent index */
bool BPlusTreePage::IsSafe(OpType optimi) {
  int tempsize = GetSize();
  if (optimi == OpType::INSERT)
  {
    return tempsize < GetMaxSize();
  }
  int tempminSize = GetMinSize() + 1;
  if (optimi == OpType::DELETE)
  { bool judge = IsLeafPage();
    return (judge) ? tempsize >= tempminSize : tempsize > tempminSize;
  }
  assert(false);//invalid area
}

} // namespace scudb
