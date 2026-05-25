#pragma once

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

class SSTableBuilder {
  public: 
    SSTableBuilder(const std::string& file_name, int cnt_per_bloc = 2);
    void add(const std::string& key, const std::string& value);
    void finish();

  private:
    std::ofstream file_;
    int cnt_per_bloc_;
    int curr_cnt_ = 0;
    uint64_t curr_offset_ = 0, last_offset_ = 0, bloc_start = 0;
    std::vector<std::pair<uint64_t, uint64_t>> offsets_;
};