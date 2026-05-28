#pragma once

namespace lsm {
  template<typename K, typename V>
  class Iterator {
    public:
      virtual ~Iterator() = default;

      virtual void first() = 0;
      virtual void next() = 0;
      virtual bool is_done() const = 0;

      virtual const K& key() const = 0;
      virtual const V& value() const = 0;

      void operator++() { next(); }
  };
}