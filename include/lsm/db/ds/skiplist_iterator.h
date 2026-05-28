#pragma once

#include "lsm/db/iterators/iterator.h"
#include "skiplist.h"

namespace lsm {
  template <typename K, typename V>
  class SkiplistIterator: public Iterator<K, V> {
    public: 
      using Node = typename SkipList<K, V>::Node;

      SkiplistIterator(const SkipList<K, V>& list): head_(list.head_), current_(nullptr) {}

      void first() override { current_ = head_; }
      void next() override { current_ = current_->forward[0]; }
      bool is_done() const override { return current_ == nullptr; }

      const K& key() const override { return current_->key; }
      const V& value() const override { return current_->value; }
    private: 
      Node* head_;
      Node* current_;
  };
}