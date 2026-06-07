#pragma once

#include <filesystem>
#include <fstream>
#include <string>

namespace lsm {
  class WALBuilder {
    public:
      WALBuilder(const std::filesystem::path& file_path);

      void set(const std::string& key, const std::string& value);

    private: 
      std::ofstream file_;
  };
}