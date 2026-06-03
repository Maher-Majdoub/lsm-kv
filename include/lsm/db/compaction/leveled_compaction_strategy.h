#pragma once

#include "lsm/db/common/sstable_metadata.h"
#include "lsm/db/sstable/sstable_manager.h"

#include <vector>

namespace lsm {
  class LeveledCompactionStrategy {
    public:
      LeveledCompactionStrategy(SSTableManager& sst_manager);

      std::pair<std::vector<SSTableMetadata>, u_short> getNextCandidates(); // { candidates, level to save in }
  
    private: 
      SSTableManager& sst_manager_;
  };
}