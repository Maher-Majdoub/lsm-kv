#pragma once

#include "lsm/db/common/types.h"
#include "format.h"

#include <filesystem>
#include <vector>

namespace lsm {
  class WALIterator;

  class WAL { 
    friend class WALIterator;

    public: 
      WAL(const std::filesystem::path& file_path);
      void load();

    private: 
      wal::Entry read_entry_(size_byte_t offset);

      std::vector<char> buff_;
      std::filesystem::path file_path_;
  };
}