#pragma once

#include "config.h"
#include "format.h"

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

namespace lsm {
  class SSTableBuilder {
    public: 
      struct BlockState {
        sstable::offset_t offset = 0;
        size_t size = 0;
  
        bool can_add_entry(size_t entry_size) const { 
          return size + entry_size <= config::sstable::MAX_BLOCK_SIZE;
        }
      };
  
      SSTableBuilder(const std::string& file_path);
      ~SSTableBuilder();
  
      void add(const std::string& key, const std::string& value);
      void finish();
  
    private:
      std::ofstream file_;
  
      BlockState current_block_;
      std::vector<sstable::index_t> index_;
  
      void flush_block_();
  };
}