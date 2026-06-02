#pragma once

#include "lsm/db/common/sstable_metadata.h"

#include <vector>

namespace lsm {
  class SSTableManager {
    public: 
      SSTableManager();

      void add(SSTableMetadata metadata);
      void remove(u_short level, const std::filesystem::path& path);

    private: 
      std::vector<std::vector<std::unique_ptr<SSTableMetadata>>> sstables_;
  };
}