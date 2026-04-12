#include "memtable.h"
#include <string>
#include <optional>
#include <lsm/ds/skiplist.h>

namespace lsm {
  MemTable::MemTable(): TOMBSTONE_("__TOMBSTONE__") {}

  void MemTable::set(const std::string& key, const std::string& value) {
    table_.insert(key, value);
  }

  std::optional<std::string> MemTable::get(const std::string& key) {
    auto res = table_.find(key);

    if (!res || *res == TOMBSTONE_) return std::nullopt;
    return res;
  }

  void MemTable::remove(const std::string& key) {
    table_.insert(key, TOMBSTONE_);
  }

  size_t MemTable::size() const {
    // TODO: implement size tracking
    return 0;
  } 
}