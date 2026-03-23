#pragma once

#include <string>
#include <optional>
#include <lsm/ds/skiplist.h>

namespace lsm {
  class MemTable {
    public: 
      MemTable() : TOMBSTONE_("__TOMBSTONE__") {}

      void set(const std::string& key, const std::string& value) {
        this->table_.insert(key, value);
      }

      std::optional<std::string> get(const std::string& key) {
        auto res = this->table_.find(key);

        if (!res || *res == TOMBSTONE_) return std::nullopt;
        return res;
      }

      void remove(const std::string& key) {
        this->table_.insert(key, TOMBSTONE_);
      }

      size_t size() const {
        // TODO: implement size tracking
        return 0;
      } 

    private:
      SkipList<std::string, std::string> table_;
      const std::string TOMBSTONE_;
  };
}