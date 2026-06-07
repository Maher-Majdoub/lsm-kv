#pragma once

#include "lsm/db/ds/skiplist.h"
#include "lsm/db/common/types.h"

#include <cstddef>
#include <string>
#include <optional>

namespace lsm {
  class Memtable {
    friend class MemtableIterator;
    
    public: 
      void set(const std::string& key, const std::string& value);
      std::optional<std::string> get(const std::string& key);
      size_byte_t size();

    private:
      Skiplist<std::string, std::string> table_;
  };
}
