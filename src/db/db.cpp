#include "lsm/db/db.h"

#include "lsm/db/common/sstable_metadata.h"
#include "lsm/db/compaction/compactor.h"
#include "lsm/db/common/config.h"
#include "lsm/db/memtable/memtable.h"
#include "lsm/db/memtable/memtable_iterator.h"
#include "lsm/db/sstable/sstable.h"
#include "lsm/db/sstable/sstable_builder.h"
#include "lsm/db/sstable/sstable_manager.h"
#include "lsm/db/wal/wal_iterator.h"
#include "lsm/db/wal/wal_manager.h"
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
    manifest_manager_(config::DB_PATH / "meta"),
    wal_manager_(config::DB_PATH / "wal"),
    compaction_strategy_(LeveledCompactionStrategy(sstable_manager_)),
    compactor_()
  {
    std::filesystem::create_directories(config::DB_PATH);

    auto sstables = manifest_manager_.load();

    sstable_manager_.set_sstables(sstables);

    while (true) {
      auto [candidates, target_level] = 
        compaction_strategy_.get_next_candidates();

      if (!candidates.size()) break;

      std::cout << "Compacting " << candidates.size() << " to level " << target_level << "\n";

      std::filesystem::path output_path =
        config::DB_PATH / ("level-" + std::to_string(target_level) + current_timestamp() + ".bin");

      auto keys = compactor_.compact(
        candidates, 
       output_path
      );

      SSTableMetadata metadata;
      metadata.level = target_level;
      metadata.path = output_path;
      metadata.min_key = keys.first;
      metadata.max_key = keys.second;

      sstable_manager_.add(metadata);
      manifest_manager_.add_sstable(std::make_unique<SSTableMetadata>(metadata));

      for (auto candidate: candidates) {
        sstable_manager_.remove(candidate.level, candidate.path);
        manifest_manager_.remove_sstable(candidate);
      }
    }

    std::optional<WALIterator> it = wal_manager_.recover();

    if (it) {
      while (!it->is_done()) {
        set(it->key(), it->value());
        it->next();
      }
    }
  }

  std::optional<std::string> DB::get(const std::string& key) {
    std::optional<std::string> value;

    // check memtable
    value = memtable_->get(key);

    if (value == std::nullopt) {
      // check sstables
      std::vector<SSTableMetadata> sst_candidates = sstable_manager_.get_candidates(key);
  
      for (const SSTableMetadata& candidate: sst_candidates ) {
        SSTable sst(candidate.path);
        value = sst.find(key);
        if (value) break;
      }
    }

    if (value && *value == config::TOMBSTONE) return std::nullopt; // key deleted
    return value;
  }

  void DB::set(const std::string& key, const std::string& value) {
    if (value == config::TOMBSTONE) {
      std::cerr << "RESERVED KEYWORD: " << value << "\n";
      return;
    }

    wal_manager_.add_entry(key, value);
    memtable_->set(key, value);
    post_update_();
  }

  void DB::remove(const std::string& key) {
    wal_manager_.add_entry(key, config::TOMBSTONE);
    memtable_->set(key, config::TOMBSTONE);
    post_update_();
  }

  void DB::post_update_() {
    if (memtable_->size() >= config::MAX_MEMTABLE_SIZE) {
      flush_memtable_();
      memtable_ = std::make_unique<Memtable>();
    }
  }

  void DB::flush_memtable_() {
    if (!memtable_->size()) return;

    const std::string file_path = config::DB_PATH / (current_timestamp() + ".bin");

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

    wal_manager_.rotate();
    sstable_manager_.add(metadata);
    manifest_manager_.add_sstable(std::make_shared<SSTableMetadata>(std::move(metadata)));

    sst.finish();
  }
}