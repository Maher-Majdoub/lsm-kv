#include "lsm/services/config_service.h"
#include <optional>
#include <cstdlib>
#include <stdexcept>
#include <string>

std::optional<std::string> ConfigService::get(const char* key) {
  return std::getenv(key);
}

std::string ConfigService::getOrThrow(const char* key) {
  const char* value = std::getenv(key);

  if (!value) 
      throw std::runtime_error(
          std::string("ENV VAR is missing: ") + key
      );

  return std::string(value);
}

