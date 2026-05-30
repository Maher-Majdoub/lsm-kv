#include "lsm/db/sstable/sstable_block_iterator.h"
#include "lsm/db/sstable/sstable.h"

#include <cstring>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace lsm {
  SSTable::SSTable(const std::string& file_path) {
    file_.open(file_path, std::ios::binary);
    
    load_footer_();
    load_indexes_();
  }
  
  SSTable::~SSTable() { 
    file_.close();
  }
  
  std::optional<std::string> SSTable::find(const std::string& key) {
    int left = 0, right = index_.size() - 1;
  
    while (left < right) {
      int middle = left + (right - left + 1) / 2;
  
      std::string current_key = read_key_(index_[middle].first);
  
      if (current_key <= key) left = middle;
      else right = middle - 1;
    }
  
    std::vector<char> buffer = read_block_(index_[left]);
    
    SSTableBlockIterator it(buffer);
    std::optional<std::string> found_value;
    
    for (it.first(); !it.is_done(); ++it) {
      if (it.key() > key) break;
      if (it.key() == key) found_value = it.value();
    }

    return found_value;
  }
  
  void SSTable::load_indexes_() {
    file_.seekg(footer_.indexes_bloc_start_offset, std::ios::beg);
  
    int count = footer_.indexes_bloc_size / sizeof(sstable::index_t);
    index_.resize(count);
  
    file_.read((char*) index_.data(), footer_.indexes_bloc_size);
  }
  
  std::string SSTable::read_key_(sstable::offset_t offset) {
    sstable::RecordHeader record_header = read_record_header_(offset);
  
    std::string key;
    
    // reading key
    file_.seekg(offset + sizeof(record_header));
    key.resize(record_header.key_size);
    file_.read(&key[0], record_header.key_size);
  
    return key;
  }
  
  void SSTable::load_footer_() {
    file_.seekg(0, std::ios::end);
    std::streamoff file_size = file_.tellg();
    file_.seekg(file_size - sizeof(footer_), std::ios::beg); 
  
    file_.read((char*) &footer_, sizeof(footer_));
  }
  
  sstable::RecordHeader SSTable::read_record_header_(sstable::offset_t offset) { 
    file_.seekg(offset, std::ios::beg);
    sstable::RecordHeader record_header;
  
    file_.read((char*) &record_header, sizeof(record_header));
  
    return record_header;
  }
  
  std::vector<char> SSTable::read_block_(const sstable::index_t& block_index) {
    const auto& [block_start, block_end] = block_index;
    size_t block_size = block_end - block_start;
  
    file_.seekg(block_start, std::ios::beg);
  
    std::vector<char> buffer(block_size);
    file_.read(buffer.data(), block_size);
    return buffer;
  }
}