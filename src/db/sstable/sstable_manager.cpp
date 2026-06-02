#include "lsm/db/sstable/sstable_manager.h"
#include "lsm/db/common/comstants.h"
#include "lsm/db/common/sstable_metadata.h"

#include <cassert>
#include <memory>
#include <vector>

namespace lsm {
  SSTableManager::SSTableManager() {
    sstables_.resize(lsm::MAX_SSTABLES_LEVELS);
  }

  void SSTableManager::add(SSTableMetadata metadata) {
    const auto level = metadata.level;
    assert(level < sstables_.size());

    sstables_[level].push_back(std::make_unique<SSTableMetadata>(std::move(metadata)));
  } 

  void SSTableManager::remove(u_short level, const std::filesystem::path& path) {
    assert(level < sstables_.size());

    std::erase_if(sstables_[level] , [&](const std::unique_ptr<SSTableMetadata>& metadata) {
      return metadata->path == path;
    });
  }

  std::vector<SSTableMetadata> SSTableManager::getCandidates(const std::string& key) {
    std::vector<SSTableMetadata> candidates;
    
    bool found = false;

    // sstables in level 0 can overlap (check all of them)
    for (auto& metadata: sstables_[0]) {
      if (metadata->min_key <= key && metadata->max_key >= key) {
        candidates.push_back(*metadata);
        found = true;
      }
    } 

    u_short curr_level = 1;
    // sstables in level 1+ are sorted and not overlapped
    while (!found && curr_level < MAX_SSTABLES_LEVELS) {
      for (auto& metadata: sstables_[curr_level]) {
        if (metadata->min_key <= key && metadata->max_key >= key) {
          candidates.push_back(*metadata);
          found = true;
          break;
        }
      }
      
      curr_level++;
    }

    return candidates;
  };
}