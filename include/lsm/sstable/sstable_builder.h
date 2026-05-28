#pragma once

#include "base_sstable.h"
#include "lsm/sstable/config.h"

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

class SSTableBuilder: BaseSStable {
  public: 
    struct BlockState {
      offset_t offset = 0;
      size_t size = 0;

      bool can_add_entry(size_t entry_size) const { 
        return size + entry_size <= lsm::config::sstable::MAX_BLOCK_SIZE;
      }
    };

    SSTableBuilder(const std::string& file_name);
    ~SSTableBuilder();

    void add(const std::string& key, const std::string& value);
    void finish();

  private:
    std::ofstream file_;

    BlockState current_block_;
    std::vector<index_t> index_;

    void flush_block_();
};