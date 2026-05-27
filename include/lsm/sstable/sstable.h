#pragma once

#include <fstream>
#include <optional>
#include<string>

using Offset = uint64_t;

class SSTable {
  public: 
    SSTable(const std::string& file_name);

    std::optional<std::string> find(const std::string& key); 

  private: 
    std::ifstream file_;
    std::uint64_t indexes_start_offset_, indexes_end_offset_;

    std::vector<std::pair<Offset, Offset>> index_;
    
    void loadIndexes_();
    std::string readKey_(Offset offset);
};