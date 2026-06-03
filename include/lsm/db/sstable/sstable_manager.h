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
      std::vector<SSTableMetadata> getCandidates(const std::string& key);
      uint64_t getSize(u_short level);

    private: 
      std::vector<std::vector<std::unique_ptr<SSTableMetadata>>> sstables_;
      std::vector<uint64_t> levels_size_;
  };
}