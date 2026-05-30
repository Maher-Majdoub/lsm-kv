#pragma once

#include "lsm/db/ds/skiplist.h"

#include <cstddef>
#include <string>
#include <optional>

namespace lsm {
  class Memtable {
    friend class MemtableIterator;
    
    public: 
      Memtable();

      void set(const std::string& key, const std::string& value);

      std::optional<std::string> get(const std::string& key);

      void remove(const std::string& key);

      size_t size();

    private:
      Skiplist<std::string, std::string> table_;
      const std::string TOMBSTONE_;
  };
}
