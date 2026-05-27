#include "lsm/sstable/sstable.h"
#include "lsm/services/config_service.h"

#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include<string>
#include <vector>

SSTable::SSTable(const std::string& file_name) {
  auto dataFolderPath = ConfigService::get("DATA_FOLDER_PATH").value_or("./data");
  std::filesystem::create_directories(dataFolderPath);

  file_.open(dataFolderPath + "/"+ file_name, std::ios::binary);
  loadIndexes_();
}


std::optional<std::string> SSTable::find(const std::string& key) {
  int left = 0, right = index_.size() - 1;

  while (left < right) {
    int middle = left + (right - left + 1) / 2;

    std::string current_key = readKey_(index_[middle].first);

    if (current_key <= key) left = middle;
    else right = middle - 1;
  }

  const auto& [bloc_start, bloc_end] = index_[left];
  size_t bloc_size = bloc_end - bloc_start;

  file_.seekg(bloc_start, std::ios::beg);

  std::vector<char> buffer(bloc_size);
  file_.read(buffer.data(), bloc_size);
  
  std::optional<std::string> found_value;
  
  size_t pos = 0;
  while (pos < buffer.size()) { 
    Offset key_size, value_size; // TODO: change the type of size
    std::memcpy(&key_size, buffer.data() + pos, sizeof(Offset));
    pos += sizeof(key_size);
    
    std::memcpy(&value_size, buffer.data() + pos, sizeof(Offset));
    pos += sizeof(value_size);
    
    std::string current_key(buffer.data() + pos, key_size);
    
    if (current_key > key) break;

    pos += key_size;

    if (current_key == key) {
      std:: string value(buffer.data() + pos, value_size);
      found_value = value;
    }

    pos += value_size;
  }

  if (found_value) {
    std::cout << "found: " << *found_value << "\n";
  }
  
  else {
    std::cout << "NOT FOUND\n";
  }

  return found_value;
}

void SSTable::loadIndexes_() {
  file_.seekg(0, std::ios::end);
  
  std::streamoff file_size = file_.tellg();
  
  file_.seekg(file_size - sizeof(Offset), std::ios::beg); 
  
  Offset start_index_offset;
  
  file_.read(reinterpret_cast<char*>(&start_index_offset), sizeof(start_index_offset));
  
  std::streamoff index_size = file_size - sizeof(Offset) - start_index_offset;

  file_.seekg(start_index_offset, std::ios::beg);
  std::vector<char> buffer(index_size);

  file_.read(buffer.data(), index_size);

  int count = buffer.size() / (sizeof(Offset) * 2);

  index_.reserve(count);

  for (int i = 0; i < count; i++) {
    Offset start, end;

    std::memcpy(
      &start, 
      buffer.data() + i * 2 * sizeof(Offset),
      sizeof(Offset)
    );

    std::memcpy(
      &end, 
      buffer.data() + (i * 2 + 1) * sizeof(Offset),
      sizeof(Offset)
    );

    index_.push_back({ start, end });
  }
}

std::string SSTable::readKey_(Offset offset) {
  // reading key size
  file_.seekg(offset, std::ios::beg);
  Offset key_size;
  file_.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));

  // reading key
  file_.seekg(offset + 2 * sizeof(Offset));
  std::string key;
  key.resize(key_size);
  file_.read(&key[0], key_size);

  return key;
} 