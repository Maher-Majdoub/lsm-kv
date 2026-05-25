#include <cstddef>
#include "lsm/sstable/sstable_builder.h"
#include "lsm/services/config_service.h"
#include <cstdint>
#include <iostream>
#include<lsm/db/db.h>
#include <fstream>

int main() {
  // lsm::DB db;

  // db.set("name", "maher");

  // auto name = db.get("name");
  // std::cout << "name: " << (name ? *name : "NOT FOUND") << "\n";

  SSTableBuilder sstable = SSTableBuilder("data.sst");

  sstable.add("hello", "world1");
  sstable.add("hello", "world2");
  sstable.add("hello", "world3");
  sstable.add("hello", "world4");
  sstable.add("hello", "world5");
  sstable.finish();

  auto dataFolderPath = ConfigService::get("DATA_FOLDER_PATH").value_or("./data");
  std::string path = dataFolderPath + "/data.sst";

  std::ifstream file(path, std::ios::binary);

  uint64_t offset = 0;

  for (int i = 0; i < 5; i++) {
    file.seekg(offset, std::ios::beg);
    uint64_t key_size, value_size;
    file.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));

    offset += sizeof(key_size);
    
    file.seekg(offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));

    offset += sizeof(value_size);
    
    std::string key;
    key.resize(key_size);
    file.read(&key[0], key_size);
    
    offset += key_size;

    std::string value; 
    value.resize(value_size);
    file.read(&value[0], value_size);

    offset += value_size;

    std::cout << "here: " << key_size << " " << value_size << " " << key << " " << value << "\n";
    if (i % 2) {
      std::cout << "here: " << offset << "\n";
    }
  }

  for (int i = 0; i < 3; i++) {
    uint64_t bloc_start, bloc_end;

    file.seekg(offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(&bloc_start), sizeof(bloc_start));

    offset += sizeof(bloc_start);

    file.seekg(offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(&bloc_end), sizeof(bloc_end));

    offset += sizeof(bloc_end);
    
    std::cout << "Bloc " << i + 1 << " : start ( " << bloc_start << " ), end ( " << bloc_end << " )\n"; 
  }
}