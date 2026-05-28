#pragma once

#include "lsm/db/iterators/iterator.h"
#include "lsm/db/ds/skiplist_iterator.h"
#include "memtable.h"

#include <string>

namespace lsm {
  class MemtableIterator: public Iterator<std::string, std::string> { 
    public: 
      MemtableIterator(MemTable& memtable): skiptlist_it_(memtable.table_) {}

      void first() override { skiptlist_it_.first(); }
      void next() override { skiptlist_it_.next(); }
      bool is_done() const override { return skiptlist_it_.is_done(); }

      const std::string& key() const override { return skiptlist_it_.key(); }
      const std::string& value() const override { return skiptlist_it_.value(); }

    private: 
      SkiplistIterator<std::string, std::string> skiptlist_it_;
  };
}