#pragma once

#include "lsm/db/manifest/manifest_entry.h"
#include "lsm/db/common/types.h"

#include <cstddef>
#include <vector>

namespace lsm {
  class Manifest {
    public: 
      Manifest(const std::filesystem::path& file_path);
      std::vector<manifest::Entry> parse();

    private:
      std::vector<char> buff_;
      void laod_manifest_(const std::filesystem::path& file_path);
      std::pair<manifest::Entry, size_byte_t> read_block_(size_byte_t start);
  }; 
}