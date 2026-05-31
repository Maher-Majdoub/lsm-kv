#pragma once

#include "format.h"

#include <cstddef>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace lsm {

  class SSTable {
    friend class SSTableIterator;

    public: 
      SSTable(const std::string& file_path);
      // ~SSTable();
  
      std::optional<std::string> find(const std::string& key); 
  
    private: 
      std::ifstream file_;
  
      std::vector<sstable::index_t> index_;
      sstable::Footer footer_;
      
      void load_footer_();
      void load_indexes_();
      std::string read_key_(sstable::offset_t offset);
      sstable::RecordHeader read_record_header_(sstable::offset_t offset);
      std::vector<char> read_block_(const sstable::index_t& block_index);
  };
}