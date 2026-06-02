#include "lsm/db/db.h"
#include "lsm/db/common/comstants.h"
#include "lsm/db/common/sstable_metadata.h"
#include "lsm/db/config.h"
#include "lsm/db/memtable/memtable.h"
#include "lsm/db/memtable/memtable_iterator.h"
#include "lsm/db/sstable/sstable_builder.h"
#include "lsm/db/sstable/sstable_manager.h"
#include "lsm/utils/time.h"

#include <cstddef>
#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

namespace lsm {
  DB::DB(): 
    memtable_(std::make_unique<Memtable>()), 
    sstable_manager_(),
    manifest_manager_(DB_PATH / "meta", sstable_manager_)
  {
    std::filesystem::create_directories(DB_PATH);

    manifest_manager_.load();
  }

  std::optional<std::string> DB::get(const std::string& _) {
    throw std::runtime_error("Method not implemented");
    // if (auto data = memtable_->get(key)) return data;

    // for (auto it = sstables_.rbegin(); it != sstables_.rend(); ++it) {
    //   if (auto data = (*it)->find(key)) return data;
    // }

    // return std::nullopt;
  }

  void DB::set(const std::string& key, const std::string& value) {
    memtable_->set(key, value);
    post_update_();
  }

  void DB::remove(const std::string& key) {
    memtable_->remove(key);
    post_update_();
  }

  void DB::post_update_() {
    if (memtable_->size() >= db::config::MAX_MEMTABLE_SIZE) {
      flush_memtable_();
      memtable_ = std::make_unique<Memtable>();
    }
  }

  void DB::flush_memtable_() {
    if (!memtable_->size()) return;

    const std::string file_path = DB_PATH / (current_timestamp() + ".bin");

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
  }
}