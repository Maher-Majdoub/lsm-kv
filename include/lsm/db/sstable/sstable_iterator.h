#pragma once

#include "lsm/db/sstable/sstable_block_iterator.h"
#include "lsm/db/iterators/iterator.h"
#include "sstable.h"

#include <string>

namespace lsm {
  class SSTableIterator: public Iterator<std::string, std::string> { 
    public: 
      SSTableIterator(SSTable& sstable);

      void first() override;
      void next() override;
      bool is_done() const override;

      const std::string& key() const override;
      const std::string& value() const override;

    private: 
      SSTable& sstable_;

      size_t p_index_;
      SSTableBlockIterator* block_it_;

      void update_block_iterator_();
  };
}