#include "lsm/db/db.h"
#include <cstddef>
#include <optional>
#include <string>

namespace lsm {
  DB::DB(size_t memtable_max_size): memtable_max_size_(memtable_max_size) {}

  std::optional<std::string> DB::get(const std::string& key) {
    return mem_table_.get(key);
  }

  void DB::set(const std::string& key, const std::string& value) {
    return mem_table_.set(key, value);
  }

  void DB::remove(const std::string& key) {
    return mem_table_.remove(key);
  }
}