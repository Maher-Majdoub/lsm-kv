#pragma once

#include "lsm/db/common/sstable_metadata.h"

#include <string>
#include <vector>

namespace lsm {
  class SSTableManager {
    public: 
      SSTableManager();

      void add(SSTableMetadata metadata);
      void remove(u_short level, const std::filesystem::path& path);
      SSTableMetadata get(u_short level, u_short pos);
      std::vector<SSTableMetadata> get_candidates(const std::string& key);
      std::vector<SSTableMetadata> get_candidates(
        const std::string& min_key, const std::string& max_key, u_short level
      );
      uint64_t getSize(u_short level);

    private: 
      std::vector<std::vector<std::unique_ptr<SSTableMetadata>>> sstables_;
      std::vector<uint64_t> levels_size_;

      bool are_key_ranges_overlapping_(
        const std::string& start1, const std::string& end1, const std::string& start2, const std::string& end2
      );
  };
}