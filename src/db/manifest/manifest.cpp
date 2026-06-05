#include "lsm/db/manifest/manifest.h"
#include "lsm/db/common/types.h"
#include "lsm/db/manifest/manifest_entry.h"

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <ios>

namespace lsm {
  Manifest::Manifest(const std::filesystem::path &file_path) {
    laod_manifest_(file_path);
  }

  void Manifest::laod_manifest_(const std::filesystem::path &file_path) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    const std::streamsize file_size = file.tellg();

    buff_.clear();
    buff_.resize(file_size);
    file.seekg(0, std::ios::beg);
    file.read(buff_.data(), file_size);
  }

  std::vector<manifest::Entry> Manifest::parse() {
    std::vector<manifest::Entry> entries;

    size_byte_t pos = 0;

    while (pos < buff_.size()) {
      auto [entry, size] = read_block_(pos);
  
      entries.push_back(std::move(entry));
      pos += size;
    }

    return entries;
  }

  std::pair<manifest::Entry, size_byte_t> Manifest::read_block_(size_byte_t start) {
    size_byte_t size = 0;

    manifest::Operation op;
    std::memcpy(&op, buff_.data() + start + size, sizeof(op));
    size += sizeof(op);

    ushort level;
    std::memcpy(&level, buff_.data() + start + size, sizeof(level));
    size += sizeof(level);

    manifest::Entry entry;
    entry.operation = op;
    entry.metadata.level = level;

    size_byte_t file_path_size, min_key_size, max_key_size;
    
    // file path
    std::memcpy(&file_path_size, buff_.data() + start + size, sizeof(file_path_size));
    size += sizeof(file_path_size);
    entry.metadata.path = std::string(buff_.data() + start + size, file_path_size);
    size += file_path_size;

    // min key
    std::memcpy(&min_key_size, buff_.data() + start + size, sizeof(min_key_size));
    size += sizeof(min_key_size);
    entry.metadata.min_key.assign(buff_.data() + start + size, min_key_size);
    size += min_key_size;

    // max key
    std::memcpy(&max_key_size, buff_.data() + start + size, sizeof(max_key_size));
    size += sizeof(max_key_size);
    entry.metadata.max_key.assign(buff_.data() + start + size, max_key_size);
    size += max_key_size;


    return { std::move(entry), size };
  }
}