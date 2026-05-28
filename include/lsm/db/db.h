#pragma once

#include "memtable/memtable.h"

#include <string>

namespace lsm {
  class DB {
    public: 
      explicit DB(size_t memtable_max_size = 64 * 1024);
      std::optional<std::string> get(const std::string& key);
      void set(const std::string& key, const std::string& value);
      void remove(const std::string& key);
      void display_memtable();
    private: 
      lsm::MemTable mem_table_;
      size_t memtable_max_size_;
  };
}
