#pragma once

#include "lsm/services/config_service.h"
#include <cstdint>
#include <filesystem>

namespace lsm {
  inline constexpr uint8_t MAX_SSTABLES_LEVELS = 7;
  inline constexpr uint8_t BASE_SSTABLE_LEVEL_SIZE = 10 * 1024 * 1024;
  inline constexpr uint8_t SSTABLE_LEVEL_MULTIPLICATION_FACTOR = 10;

  inline const std::filesystem::path DB_PATH = ConfigService::get("DATA_FOLDER_PATH").value_or("./data");
}