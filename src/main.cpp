#include "lsm/db/db.h"
#include "lsm/db/manifest/manifest_manager.h"
#include "lsm/services/config_service.h"

#include <cassert>
#include <cstddef>
#include <iostream>

int main() {
  lsm::DB db;

  int n = 1000;

  for (int i = 0; i < n; i++) {
    std::string key = "key" + std::to_string(i), value = "value" + std::to_string(i);
    db.set(key, value);
  }

  db.display_memtable();

  // // auto result = db.get("key0");
  // // if (result) {
  // //   std::cout << "Found: " << *result << "\n";
  // // }
  // // else {
  // //   std::cout << "Not Found\n";
  // // }
  
  // std::vector<std::string> names = {
  //   "/1780177883784991.bin",
  //   "/1780177883785626.bin", 
  //   "/1780177883786013.bin",
  //   "/1780177883786366.bin",
  //   "/1780177883786742.bin",
  //   "/1780177883787089.bin",
  // };

  // std::vector<lsm::SSTableMetadata> tables;
  
  // for (auto name: names) {
  //   lsm::SSTableMetadata dt;
  //   dt.path = lsm::ConfigService::get("DATA_FOLDER_PATH").value_or("./data") + name;
  //   tables.push_back(dt);
  // }

  // lsm::Compactor compactor;

  // compactor.compact(
  //   tables, 
  //   lsm::ConfigService::get("DATA_FOLDER_PATH").value_or("./data") + "/out.bin"
  // );

  // lsm::SSTable table(lsm::ConfigService::get("DATA_FOLDER_PATH").value_or("./data") + "/out.bin");

  // // lsm::SSTableIterator it(table);
  // // for (it.first(); !it.is_done(); ++it) {
  // //   std::cout << it.key() << " " << it.value() << "\n";
  // // }

  // for (int i = 0; i < 500; i++) { 
  //   assert(table.find("key" + std::to_string(i)));
  // }

  // lsm::ManifestManager manifest(lsm::ConfigService::get("DATA_FOLDER_PATH").value_or("./data") + "/meta");
  
}