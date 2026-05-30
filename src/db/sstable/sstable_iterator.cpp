#include "lsm/db/sstable/sstable_block_iterator.h"
#include "lsm/db/sstable/sstable_iterator.h"
#include "lsm/db/sstable/sstable.h"

#include <cstddef>
#include <vector>

namespace lsm {
  SSTableIterator::SSTableIterator(SSTable& sstable): sstable_(sstable) {}

  void SSTableIterator::first() {
    p_index_ = 0;
    update_block_iterator_();
  }

  void SSTableIterator::next() {
    if (block_it_->is_done()) { 
      p_index_++; 
      update_block_iterator_();
    } else {
      ++block_it_;
    }
  }

  bool SSTableIterator::is_done() const {
    return p_index_ >= sstable_.index_.size();
  }

  const std::string& SSTableIterator::key() const {
    return block_it_->key();
  }

  const std::string& SSTableIterator::value() const {
    return block_it_->value();
  }

  void SSTableIterator::update_block_iterator_() {
    delete block_it_;

    if (p_index_ >= sstable_.index_.size()) {
      block_it_ = nullptr;
      return;
    }

    std::vector<char> buffer = sstable_.read_block_(sstable_.index_[p_index_]);
    block_it_ = new SSTableBlockIterator(buffer);
    block_it_->first();
  }
}