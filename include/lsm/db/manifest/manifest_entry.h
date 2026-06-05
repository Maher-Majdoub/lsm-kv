#pragma once

#include "lsm/db/common/sstable_metadata.h"

#include <cstdint>

namespace lsm::manifest {
   enum class Operation: uint8_t {
    ADD = 0x01,
    REMOVE = 0x02,
  };

  struct Entry {
    Operation operation;
    SSTableMetadata metadata;
  };
}