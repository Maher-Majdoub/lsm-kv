#pragma once

#include "lsm/db/common/types.h"
#include "lsm/services/config_service.h"

#include <cstddef>
#include <filesystem>
#include <string>

namespace lsm::config {
  constexpr size_byte_t MAX_MEMTABLE_SIZE = 20 * 1024;

  constexpr uint8_t MAX_SSTABLES_LEVELS = 7;
  constexpr uint8_t BASE_SSTABLE_LEVEL_SIZE = 200;
  constexpr uint8_t SSTABLE_LEVEL_MULTIPLICATION_FACTOR = 2;

  inline std::filesystem::path DB_PATH = ConfigService::get("DATA_FOLDER_PATH").value_or("./data");

  constexpr std::string TOMBSTONE = "__TOMBSTONE__";
}