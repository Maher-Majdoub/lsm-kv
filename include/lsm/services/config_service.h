#pragma once

#include <optional>
#include <string>

class ConfigService {
  public: 
    static std::optional<std::string> get(const char* key);
    static std::string getOrThrow(const char* key);
};