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
}