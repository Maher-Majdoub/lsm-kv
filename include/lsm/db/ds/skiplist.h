#pragma once

#include <cstddef>
#include <random>
#include <cstdint>
#include <optional> 

namespace lsm {
  template<typename K, typename V>
  class SkiplistIterator;

  template <typename K, typename V>
  class Skiplist {
    friend class SkiplistIterator<K, V>;

    public:
      struct Node {
        K key;
        V value;
        uint64_t seq;
        Node** forward;

        Node(const K& k, const V& v, uint64_t s, int level)
          : key(k), value(v), seq(s), forward(new Node*[level + 1]()) {}
      };

      Skiplist(int max_level = 16, float p = 0.5);

      ~Skiplist();

      // Non-copyable and non-movable: prevents double deletion of nodes
      Skiplist(const Skiplist&) = delete;
      Skiplist& operator=(const Skiplist&) = delete;
      Skiplist(Skiplist&&) = delete;
      Skiplist& operator=(Skiplist&&) = delete;

      void insert(const K& key, const V& value);

      std::optional<V> find(const K& key);

      size_t size();

    private: 
      int randomLevel();

      Node* findNode_(const K& key);

      Node* head_;
      const int max_level_;
      const float p_;
      uint64_t seq_;
      mutable std::mt19937 engine_;
      size_t bytes_ = 0;
  };
}


#include "skiplist.tpp"