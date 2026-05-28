#pragma once

#include "base_sstable.h"

#include <cstddef>
#include <fstream>
#include <optional>
#include<string>
#include <vector>

namespace lsm {
  class SSTable: BaseSStable {
    public: 
      SSTable(const std::string& file_name);
      ~SSTable();
  
      std::optional<std::string> find(const std::string& key); 
  
    private: 
      std::ifstream file_;
  
      std::vector<index_t> index_;
      Footer footer_;
      
      void load_footer_();
      void load_indexes_();
      std::string read_key_(offset_t offset);
      RecordHeader read_record_header_(offset_t offset);
      std::vector<char> read_block_(const index_t& block_index);
  };
}