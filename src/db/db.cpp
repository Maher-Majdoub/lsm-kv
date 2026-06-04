#include "lsm/db/db.h"
#include "lsm/db/common/constants.h"
#include "lsm/db/common/sstable_metadata.h"
#include "lsm/db/compaction/compactor.h"
#include "lsm/db/config.h"
#include "lsm/db/memtable/memtable.h"
#include "lsm/db/memtable/memtable_iterator.h"
#include "lsm/db/sstable/sstable.h"
#include "lsm/db/sstable/sstable_builder.h"
#include "lsm/db/sstable/sstable_manager.h"
#include "lsm/utils/time.h"

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace lsm {
  DB::DB(): 
    memtable_(std::make_unique<Memtable>()), 
    sstable_manager_(),
    manifest_manager_(DB_PATH / "meta", sstable_manager_),
    compaction_strategy_(LeveledCompactionStrategy(sstable_manager_)),
    compactor_()
  {
    std::filesystem::create_directories(DB_PATH);
    manifest_manager_.load();

    while (true) {
      auto [candidates, target_level] = 
        compaction_strategy_.get_next_candidates();

      if (!candidates.size()) break;

      std::cout << "Compacting " << candidates.size() << " to level " << target_level << "\n";

      compactor_.compact(
        candidates, 
        DB_PATH / ("level-" + std::to_string(target_level) + current_timestamp() + ".bin")
      );
      
      for (auto candidate: candidates) {
        sstable_manager_.remove(candidate.level, candidate.path);
      }
    }
  }

  std::optional<std::string> DB::get(const std::string& key) {
    // check memtable
    if (auto data = memtable_->get(key)) return data;

    // check sstables
    std::vector<SSTableMetadata> sst_candidates = sstable_manager_.get_candidates(key);
    for (const SSTableMetadata& candidate: sst_candidates ) {
      SSTable sst(candidate.path);
      
      std::optional<std::string> value = sst.find(key);

      if (value) return *value;
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

    SSTableMetadata metadata;
    metadata.level = 0;
    metadata.path = file_path;

    it.first();
    metadata.min_key = it.key();

    // TODO: optimize the max key searching (it should take the last one directly)
    for (; !it.is_done(); ++it) {
      sst.add(it.key(), it.value());
      metadata.max_key = it.key();
    }

    manifest_manager_.add_sstable(std::make_unique<SSTableMetadata>(std::move(metadata)));

    sst.finish();
  }
}