#pragma once

#include "lsm/db/common/types.h"
#include "lsm/db/iterators/iterator.h"
#include "lsm/db/wal/wal.h"


#include <memory>
#include <string>

namespace lsm {
  class WALIterator: public Iterator<std::string, std::string> {
    public:
      WALIterator(WAL& wal);

      void first() override;
      void next() override;
      bool is_done() const override;

      const std::string& key() const override;
      const std::string& value() const override;

    private: 
      std::unique_ptr<WAL> wal_;
      size_byte_t p_index;
      wal::Entry curr_entry_;
  };
}