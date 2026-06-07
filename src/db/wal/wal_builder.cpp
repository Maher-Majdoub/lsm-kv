#include "lsm/db/wal/wal_builder.h"
#include "lsm/db/common/types.h"

#include <filesystem>
#include <iostream>
#include <string>

namespace lsm {
  WALBuilder::WALBuilder(const std::filesystem::path& file_path) {
    file_.open(file_path, std::ios::binary | std::ios::app);
  }

  void WALBuilder::set(const std::string& key, const std::string& value) {
    // [key_size][key][value_size][size]

    size_byte_t key_size = key.size();
    size_byte_t value_size = value.size();

    file_.write((char*) &key_size, sizeof(key_size));
    file_.write(key.data(), key_size);
    file_.write((char*) &value_size, sizeof(value_size));
    file_.write(value.data(), value_size);
    file_.flush();
  }
}