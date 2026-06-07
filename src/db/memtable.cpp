#pragma once

#include <lsm/db/memtable/memtable.h>
#include <string>
#include <optional>

namespace lsm {
  void Memtable::set(const std::string& key, const std::string& value) {
    table_.insert(key, value);
  }

  std::optional<std::string> Memtable::get(const std::string& key) {
    return table_.find(key);
  }

  size_byte_t Memtable::size() {
    return table_.size();
  } 
}