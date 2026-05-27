#include "lsm/sstable/sstable_builder.h"

#include <cstddef>
#include <string>
#include <iostream>

SSTableBuilder::SSTableBuilder(const std::string& file_name, int cnt_per_block): 
  BaseSStable(file_name), cnt_per_block_(cnt_per_block) 
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

  file_.write((char*) &record_header, sizeof(record_header));
  file_.write(key.data(), key_size);
  file_.write(value.data(), value_size);

  curr_cnt_ ++;

  curr_offset_ += sizeof(record_header) + key_size + value_size;

  if (curr_cnt_ >= cnt_per_block_) {
    index_.push_back({ block_start, curr_offset_ } );
    curr_cnt_ = 0;
    block_start = curr_offset_;
  }
}

void SSTableBuilder::finish() {
  if (curr_cnt_) {
    index_.push_back({ block_start,  curr_offset_ } );
  }

  size_t indexes_block_size = index_.size() * sizeof(index_t);
  file_.write((char *) index_.data(), indexes_block_size);

  Footer footer(curr_offset_, indexes_block_size);
  
  file_.write((char*) &footer, sizeof(footer));
  file_.close();
}