#pragma once

#include "lsm/db/iterators/iterator.h"
#include "lsm/db/common/types.h"

#include <cstddef>
#include <string>
#include <vector>

namespace lsm {
  class SSTableBlockIterator: public Iterator<std::string, std::string> {
    public: 
      SSTableBlockIterator(const std::vector<char> buffer);

      void first() override;
      void next() override;
      bool is_done() const override;

      const std::string& key() const override;
      const std::string& value() const override;

    private: 
      const std::vector<char> buffer_;
      std::string current_key_, current_value_;
      size_byte_t pos_;
      size_byte_t current_size_;

      void update_();
  };
}