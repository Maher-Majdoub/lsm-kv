#pragma once

#include "lsm/db/common/sstable_metadata.h"

#include <vector>

namespace lsm {
  class Compactor {
    public: 
      std::pair<std::string, std::string> compact(std::vector<SSTableMetadata>& input, std::filesystem::path output_path);
  }; 
}