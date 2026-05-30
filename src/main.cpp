#include "lsm/db/db.h"
#include "lsm/db/sstable/sstable.h"
#include "lsm/db/sstable/sstable_iterator.h"
#include "lsm/services/config_service.h"

#include <cstddef>
#include <iostream>

int main() {
  lsm::DB db;

  // int n = 1000;

  // for (int i = 0; i < n; i++) {
  //   std::string key = "key" + std::to_string(i), value = "value" + std::to_string(i);
  //   db.set(key, value);
  // }

  // auto result = db.get("key0");
  // if (result) {
  //   std::cout << "Found: " << *result << "\n";
  // }
  // else {
  //   std::cout << "Not Found\n";
  // }

  lsm::SSTable table(lsm::ConfigService::get("DATA_FOLDER_PATH").value_or("./data") + "/1780159275039280.bin");

  lsm::SSTableIterator it(table);
  for (it.first(); !it.is_done(); ++it) {
    std::cout << it.key() << " " << it.value() << "\n";
  }
}