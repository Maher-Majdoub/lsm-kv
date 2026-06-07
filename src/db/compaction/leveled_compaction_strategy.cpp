#include "lsm/db/common/sstable_metadata.h"
#include "lsm/db/compaction/leveled_compaction_strategy.h"
#include "lsm/db/common/config.h"
#include "lsm/db/sstable/sstable_manager.h"

#include <cmath>
#include <iterator>
#include <utility>
#include <vector>

namespace lsm { 
  LeveledCompactionStrategy::LeveledCompactionStrategy(SSTableManager& sst_manager): 
    sst_manager_(sst_manager) {}

  std::pair<std::vector<SSTableMetadata>, u_short> LeveledCompactionStrategy::get_next_candidates() {
    std::vector<SSTableMetadata> candidates;
    u_short target_level = 0;

    for (u_short level = 0; level < config::MAX_SSTABLES_LEVELS - 1; level++) {
      uint64_t size = sst_manager_.getSize(level);
      
      if (size > get_max_level_size_(level)) {
        SSTableMetadata metadata = sst_manager_.get(level, 0);

        std::vector<SSTableMetadata> next_level_candidates = sst_manager_.get_candidates(
          metadata.min_key, metadata.max_key, level + 1
        );

        candidates.push_back(std::move(metadata));
        
        candidates.insert(
          candidates.end(),
          std::make_move_iterator(next_level_candidates.begin()),
          std::make_move_iterator(next_level_candidates.end())
        );

        target_level = level + 1; // insert into the next level 
        break;
      }
    }

    return { std::move(candidates), target_level };
  }
  
  uint64_t LeveledCompactionStrategy::get_max_level_size_(u_short level) {
    return config::BASE_SSTABLE_LEVEL_SIZE * std::pow(config::SSTABLE_LEVEL_MULTIPLICATION_FACTOR, level);
  }
}