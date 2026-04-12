#pragma once

#include <string>
#include <optional>
#include <lsm/ds/skiplist.h>

namespace lsm {
  class MemTable {
    public: 
      MemTable();

      void set(const std::string& key, const std::string& value);

      std::optional<std::string> get(const std::string& key);

      void remove(const std::string& key);

      size_t size() const;

    private:
      SkipList<std::string, std::string> table_;
      const std::string TOMBSTONE_;
  };
}

#include "memtable.tpp"