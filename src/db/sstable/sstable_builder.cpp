#include "lsm/db/sstable/sstable_builder.h"

#include <cstddef>
#include <string>
#include <iostream>

namespace lsm {
  SSTableBuilder::SSTableBuilder(const std::string& file_path): 
    BaseSStable(file_path)
  {
    file_.open(file_path_, std::ios::binary);
  }
  
  SSTableBuilder::~SSTableBuilder() {
    file_.close();
  }
  
  void SSTableBuilder::add(const std::string& key, const std::string& value) {
    RecordHeader record_header(key.size(), value.size());
    
    size_t key_size = key.size();
    size_t value_size = value.size();
  
    size_t entry_size = sizeof(record_header) + key_size + value_size;
  
    if (!current_block_.can_add_entry(entry_size)) flush_block_();
  
    file_.write((char*) &record_header, sizeof(record_header));
    file_.write(key.data(), key_size);
    file_.write(value.data(), value_size);
  
    current_block_.size += entry_size;
  }
  
  void SSTableBuilder::finish() {
    flush_block_();
  
    size_t indexes_block_size = index_.size() * sizeof(index_t);
  
    Footer footer(file_.tellp(), indexes_block_size);
  
    file_.write((char *) index_.data(), indexes_block_size);
    file_.write((char*) &footer, sizeof(footer));
    
    file_.flush();
    file_.close();
  }
  
  void SSTableBuilder::flush_block_() {
    if (!current_block_.size) return;
  
    offset_t block_end = current_block_.offset + current_block_.size;
  
    index_.push_back({ current_block_.offset, block_end } );
    
    // new block
    current_block_.offset = block_end;
    current_block_.size = 0;
  }
}  
