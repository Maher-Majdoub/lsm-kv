#include "lsm/db/wal/wal.h"
#include "lsm/db/common/types.h"

#include <cassert>
#include <fstream>
#include <iostream>

namespace lsm {
  WAL::WAL(const std::filesystem::path& file_path): file_path_(file_path) {}

  void WAL::load() {
    std::ifstream file(file_path_, std::ios::binary | std::ios::ate);
    const std::streamsize file_size = file.tellg();

    buff_.clear();
    buff_.resize(file_size);
    file.seekg(0, std::ios::beg);
    file.read(buff_.data(), file_size);
  }

  wal::Entry WAL::read_entry_(size_byte_t offset) {
    assert(offset < buff_.size());

    size_byte_t curr_offset = offset;
    size_byte_t key_size, value_size;

    std::memcpy(&key_size, buff_.data() + curr_offset, sizeof(key_size));
    curr_offset += sizeof(key_size);
    std::string key(buff_.begin() + curr_offset, buff_.begin() + curr_offset + key_size);
    curr_offset += key_size;

    std::memcpy(&value_size, buff_.data() + curr_offset, sizeof(value_size));
    curr_offset += sizeof(value_size);
    std::string value(buff_.begin() + curr_offset, buff_.begin() + curr_offset + key_size);

    return wal::Entry(key, value, key_size + value_size + 2 * sizeof(key_size));
  }
}