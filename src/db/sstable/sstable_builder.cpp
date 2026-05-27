#include "lsm/sstable/sstable_builder.h"
#include "lsm/services/config_service.h"
#include <cstddef>
#include <string>
#include <filesystem>
#include <iostream>


SSTableBuilder::SSTableBuilder(const std::string& file_name, int cnt_per_bloc): cnt_per_bloc_(cnt_per_bloc) {
  auto dataFolderPath = ConfigService::get("DATA_FOLDER_PATH").value_or("./data");
  std::filesystem::create_directories(dataFolderPath);

  file_.open(dataFolderPath + "/"+ file_name, std::ios::binary);
}

void SSTableBuilder::add(const std::string& key, const std::string& value) {
  uint64_t key_size = key.size();
  uint64_t value_size = value.size();

  file_.write(reinterpret_cast<char*>(&key_size), sizeof(key_size));
  file_.write(reinterpret_cast<char*>(&value_size), sizeof(value_size));
  file_.write(key.data(), key_size);
  file_.write(value.data(), value_size);

  curr_cnt_ ++;

  curr_offset_ += sizeof(key_size) + sizeof(value_size) + key_size + value_size;

  if (curr_cnt_ >= cnt_per_bloc_) {
    offsets_.push_back({ bloc_start, curr_offset_ } );
    curr_cnt_ = 0;
    bloc_start = curr_offset_;
  }
}

void SSTableBuilder::finish() {
  if (curr_cnt_) {
    offsets_.push_back({ bloc_start,  curr_offset_ } );
  }

  for (const auto& [start, end]: offsets_) {
    std::cout << "Test: " << start << " " << end << "\n";
    file_.write(reinterpret_cast<const char*>(&start), sizeof(start));
    file_.write(reinterpret_cast<const char*>(&end), sizeof(end));
  }

  file_.write(
    reinterpret_cast<char*>(&curr_offset_),
    sizeof(curr_offset_)
  ); // indexes bloc starts at this position
  
  file_.close();
}