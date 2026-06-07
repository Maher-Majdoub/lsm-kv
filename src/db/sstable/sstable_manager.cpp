#include "lsm/db/sstable/sstable_manager.h"
#include "lsm/db/common/config.h"
#include "lsm/db/common/sstable_metadata.h"

#include <cassert>
#include <filesystem>
#include <iterator>
#include <memory>
#include <vector>

namespace lsm {
  SSTableManager::SSTableManager() {
    sstables_.resize(config::MAX_SSTABLES_LEVELS);
    levels_size_.resize(config::MAX_SSTABLES_LEVELS, 0);
  }

  void SSTableManager::add(SSTableMetadata metadata) {
    const auto level = metadata.level;
    assert(level < sstables_.size());

    levels_size_[level] += std::filesystem::file_size(metadata.path);

    sstables_[level].push_back(std::make_shared<SSTableMetadata>(std::move(metadata)));
  } 

  void SSTableManager::remove(u_short level, const std::filesystem::path& path) {
    assert(level < sstables_.size());
    assert(std::filesystem::exists(path)); // deletion should be handled by the manager

    std::erase_if(sstables_[level] , [&](const std::shared_ptr<SSTableMetadata>& metadata) {
      return metadata->path == path;
    });

    levels_size_[level] -= std::filesystem::file_size(path);
    std::filesystem::remove(path);
  }

  void SSTableManager::set_sstables(std::vector<std::vector<std::shared_ptr<SSTableMetadata>>>& sstables) {
    sstables_.assign(std::move_iterator(sstables.begin()), std::move_iterator(sstables.end()));

    for (u_short level = 0; level < config::MAX_SSTABLES_LEVELS; level++) {
      levels_size_[level] = 0;

      for (auto metadata: sstables_[level]) 
        levels_size_[level] += std::filesystem::file_size(metadata->path);
    }
  }

  std::vector<SSTableMetadata> SSTableManager::get_candidates(const std::string& key) {
    std::vector<SSTableMetadata> candidates;
    bool found = false;

    // sstables in level 0 can overlap (check all of them)
    for (auto& metadata: sstables_[0]) {      
      if (are_key_ranges_overlapping_(
        metadata->min_key, metadata->max_key,key, key
      )) {
        candidates.push_back(*metadata);
        found = true;
      }
    } 

    u_short curr_level = 1;
    // sstables in level 1+ are sorted and not overlapped
    while (!found && curr_level < config::MAX_SSTABLES_LEVELS) {
      if (sstables_[curr_level].size()) {
        for (auto& metadata: sstables_[curr_level]) {
          if (are_key_ranges_overlapping_(
            metadata->min_key, metadata->max_key, key, key
          )) {
            candidates.push_back(*metadata);
            found = true;
            break;
          }
        }
      }

      curr_level++;
    }

    return candidates;
  };

  std::vector<SSTableMetadata> SSTableManager::get_candidates(
    const std::string& min_key, const std::string& max_key, u_short level
  ) {
    assert(level < config::MAX_SSTABLES_LEVELS);
    std::vector<SSTableMetadata> candidates;

    bool found = false;

    for (auto& metadata: sstables_[level]) {
      if (are_key_ranges_overlapping_(
        metadata->min_key, metadata->max_key, min_key, max_key
      )) {
        candidates.push_back(*metadata);
        found = true;
      }

      else if (found) break; // next sstables surely don't overlap with this table
    }

    return candidates;
  };

  uint64_t SSTableManager::getSize(u_short level) {
    assert(level < config::MAX_SSTABLES_LEVELS);
    return levels_size_[level];
  }

  SSTableMetadata SSTableManager::get(u_short level, u_short pos) {
    assert(level < config::MAX_SSTABLES_LEVELS);
    assert(pos < sstables_[level].size());

    return *sstables_[level][pos];
  }

  bool SSTableManager::are_key_ranges_overlapping_(
    const std::string& start1, const std::string& end1, const std::string& start2, const std::string& end2
  ) { 
    // TODO: it seems that there is a better way to handle this :)
    return (start1 >= start2 && start1 <= end2) 
      || (end1 >= start2 && end1 <= end2) 
      || (start2 >= start1 && start2 <= end1) 
      || (end2 >= start1 && end2 <= end1);
  } 
}