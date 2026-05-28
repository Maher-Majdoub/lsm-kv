#include "lsm/db/db.h"
#include "lsm/db/memtable/memtable_iterator.h"

#include <cstddef>
#include <iostream>
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

  void DB::display_memtable() {
    MemtableIterator it(mem_table_);

    for (it.first(); !it.is_done(); ++it) {
      std::cout << it.key() << " " << it.value() << "\n";
    }
  }
}