#include "lsm/db/wal/wal_iterator.h"
#include "lsm/db/wal/wal.h"

#include <cassert>
#include <memory>

namespace lsm {
  WALIterator::WALIterator(WAL& wal): wal_(std::make_unique<WAL>(std::move(wal))), p_index(0) {
    wal_->load();
  }

  void WALIterator::first() { p_index = 0; };
  
  void WALIterator::next() { 
    assert(!is_done());

    p_index += curr_entry_.size;    
    
    if (is_done()) curr_entry_.size = 0;
    else curr_entry_ = wal_->read_entry_(p_index);
  } 

  bool WALIterator::is_done() const { 
    return p_index >= wal_->buff_.size();
  }

  const std::string& WALIterator::key() const { return curr_entry_.key; }
  const std::string& WALIterator::value() const { return curr_entry_.value; }
}