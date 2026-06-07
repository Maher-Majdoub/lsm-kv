#include "lsm/db/common/types.h"

#include <string>

namespace lsm::wal {
  struct Entry {
    std::string key;
    std::string value;
    size_byte_t size;

    Entry(): size(0) {}
    
    Entry(std::string& key, std::string& value, size_byte_t size): 
      key(std::move(key)),
      value(std::move(value)),
      size(size) {}
  };
}