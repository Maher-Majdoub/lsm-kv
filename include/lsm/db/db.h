#pragma once

#include "lsm/db/manifest/manifest_manager.h"
#include "lsm/db/sstable/sstable_manager.h"
#include "memtable/memtable.h"

#include <memory>
#include <string>

namespace lsm {
  class DB {
    public: 
      DB();
      std::optional<std::string> get(const std::string& key);
      void set(const std::string& key, const std::string& value);
      void remove(const std::string& key);
    private: 
      std::unique_ptr<Memtable> memtable_;

      SSTableManager sstable_manager_;
      ManifestManager manifest_manager_;

      void post_update_();
      void flush_memtable_();
  };
}
