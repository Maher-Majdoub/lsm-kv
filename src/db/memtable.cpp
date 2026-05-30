#pragma once

#include <lsm/db/memtable/memtable.h>
#include <string>
#include <optional>

namespace lsm {
  Memtable::Memtable(): TOMBSTONE_("__TOMBSTONE__") {}

  void Memtable::set(const std::string& key, const std::string& value) {
    table_.insert(key, value);
  }

  std::optional<std::string> Memtable::get(const std::string& key) {
    std::optional<std::string> res = table_.find(key);

    if (!res || *res == TOMBSTONE_) return std::nullopt;
    return res;
  }

  void Memtable::remove(const std::string& key) {
    table_.insert(key, TOMBSTONE_);
  }

  size_t Memtable::size() {
    return table_.size();
  } 
}