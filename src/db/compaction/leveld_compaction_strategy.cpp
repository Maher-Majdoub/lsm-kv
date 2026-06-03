#include "lsm/db/compaction/leveled_compaction_strategy.h"
#include "lsm/db/common/comstants.h"

#include <utility>
#include <vector>

namespace lsm { 
  LeveledCompactionStrategy::LeveledCompactionStrategy(SSTableManager& sst_manager): 
    sst_manager_(sst_manager) {}

  std::pair<std::vector<SSTableMetadata>, u_short> LeveledCompactionStrategy::getNextCandidates() {
    std::vector<SSTableMetadata> candidates;
    u_short target_level = 0;

    for (u_short level = 0; level < MAX_SSTABLES_LEVELS - 1; level++) {
      
    }


    return { std::move(candidates), target_level };
  } 
}