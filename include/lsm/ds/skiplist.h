#pragma once

#include <cstddef>
#include <random>
#include <cstdint>
#include <optional> 

template <typename K, typename V>
struct Node {
  K key;
  V value;
  uint64_t seq;
  Node** forward;

  Node(const K& k, const V& v, uint64_t s, int level)
    : key(k), value(v), seq(s), forward(new Node*[level + 1]()) {}
};

namespace lsm {
  template <typename K, typename V>
  class SkipList {
    public:
      SkipList(int max_level = 16, float p = 0.5);

      ~SkipList();

      // Non-copyable and non-movable: prevents double deletion of nodes
      SkipList(const SkipList&) = delete;
      SkipList& operator=(const SkipList&) = delete;
      SkipList(SkipList&&) = delete;
      SkipList& operator=(SkipList&&) = delete;

      void insert(const K& key, const V& value);

      std::optional<V> find(const K& key);

      size_t size();

    private: 
      int randomLevel();

      Node<K, V>* findNode_(const K& key);

      Node<K, V>* head_;
      const int max_level_;
      const float p_;
      uint64_t seq_;
      mutable std::mt19937 engine_;
      size_t bytes_ = 0;
  };
}


#include "skiplist.tpp"