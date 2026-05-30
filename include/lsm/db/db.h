#pragma once

#include "lsm/db/sstable/sstable.h"
#include "memtable/memtable.h"

#include <string>
#include <vector>

namespace lsm {
  class DB {
    public: 
      DB();
      std::optional<std::string> get(const std::string& key);
      void set(const std::string& key, const std::string& value);
      void remove(const std::string& key);
      void display_memtable();
    private: 
      Memtable* memtable_;
      size_t memtable_max_size_;
      std::vector<SSTable*> sstables_;
      std::string sstables_folder_path_;

      void post_update_();
      void flush_memtable_();
  };
}
