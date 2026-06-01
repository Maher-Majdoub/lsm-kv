#include "lsm/db/db.h"
#include "lsm/db/common/sstable_metadata.h"
#include "lsm/db/config.h"
#include "lsm/db/memtable/memtable.h"
#include "lsm/db/memtable/memtable_iterator.h"
#include "lsm/db/sstable/sstable_builder.h"
#include "lsm/services/config_service.h"
#include "lsm/db/sstable/sstable.h"
#include "lsm/utils/time.h"

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

namespace lsm {
  DB::DB(): 
    memtable_(std::make_unique<Memtable>()), 
    sstables_folder_path_(ConfigService::get("DATA_FOLDER_PATH").value_or("./data")), 
    manifest_manager_(sstables_folder_path_ + "/meta") 
  {
    std::filesystem::create_directories(sstables_folder_path_);

    for (const auto &entry : std::filesystem::directory_iterator(sstables_folder_path_)) {
      sstables_.push_back(new SSTable(entry.path()));
    }
  }

  std::optional<std::string> DB::get(const std::string& key) {
    if (auto data = memtable_->get(key)) return data;

    for (auto it = sstables_.rbegin(); it != sstables_.rend(); ++it) {
      if (auto data = (*it)->find(key)) return data;
    }

    return std::nullopt;
  }

  void DB::set(const std::string& key, const std::string& value) {
    memtable_->set(key, value);
    post_update_();
  }

  void DB::remove(const std::string& key) {
    memtable_->remove(key);
    post_update_();
  }

  void DB::display_memtable() {
    MemtableIterator it(*memtable_);

    for (it.first(); !it.is_done(); ++it) {
      std::cout << it.key() << " " << it.value() << "\n";
    }
  }

  void DB::post_update_() {
    if (memtable_->size() >= db::config::MAX_MEMTABLE_SIZE) {
      flush_memtable_();
      memtable_ = std::make_unique<Memtable>();
    }
  }

  void DB::flush_memtable_() {
    if (!memtable_->size()) return;

    const std::string file_path = sstables_folder_path_ + "/" + current_timestamp() + ".bin";

    SSTableBuilder sst(file_path);
    MemtableIterator it(*memtable_);

    for (it.first(); !it.is_done(); ++it) {
      sst.add(it.key(), it.value());
    }


    SSTableMetadata metadata;
    metadata.level = 0;
    metadata.path = file_path;
    metadata.max_key = "TODO: FIX THIS";
    metadata.min_key = "TODO: FIX THIS";

    manifest_manager_.add_sstable(std::make_unique<SSTableMetadata>(std::move(metadata)));

    sst.finish();

    sstables_.push_back(new SSTable(file_path));
  }
}