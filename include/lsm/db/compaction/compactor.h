#pragma once

#include "lsm/db/common/sstable_metadata.h"

#include <vector>

namespace lsm {
  class Compactor {
    public: 
      void compact(std::vector<SSTableMetadata>& input, std::filesystem::path output_path);
  }; 
}