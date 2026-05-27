#pragma once

#include "base_sstable.h"

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

class SSTableBuilder: BaseSStable {
  public: 
    SSTableBuilder(const std::string& file_name, int cnt_per_block = 2);
    ~SSTableBuilder();

    void add(const std::string& key, const std::string& value);
    void finish();

  private:
    std::ofstream file_;
    int cnt_per_block_;
    int curr_cnt_ = 0;
    offset_t curr_offset_ = 0, block_start = 0;
    std::vector<index_t> index_;
};