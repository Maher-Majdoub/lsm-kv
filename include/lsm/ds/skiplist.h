#pragma once

#include <vector>
#include <random>
#include <cstdint>
#include <optional> 

template <typename K, typename V>
struct Node {
  K key;
  V value;
  uint64_t seq;
  std::vector<Node*> forward;

  Node(const K& k, const V& v, uint64_t s, int level)
    : key(k), value(v), seq(s), forward(level + 1, nullptr) {}
};

namespace lsm {
  template <typename K, typename V>
  class SkipList {
    public:
      SkipList(int max_level = 16, float p = 0.5) : max_level_(max_level), p_(p), seq_(0) {
        head_ = new Node<K, V>(K{}, V{}, 0, max_level_);
        engine_.seed(std::random_device{}());
      }

      ~SkipList() {
        Node<K, V>* node = head_;
        while (node) {
          Node<K, V>* next = node->forward[0];
          delete node;
          node = next;
        }
      }

      // Non-copyable and non-movable: prevents double deletion of nodes
      SkipList(const SkipList&) = delete;
      SkipList& operator=(const SkipList&) = delete;
      SkipList(SkipList&&) = delete;
      SkipList& operator=(SkipList&&) = delete;

      void insert(const K& key, const V& value) {
        std::vector<Node<K, V>*> update(max_level_ + 1, nullptr);

        Node<K, V>* current_node = head_;
        int cur_level = max_level_;

        while (cur_level >= 0) {
          Node<K, V>* next_node = current_node->forward[cur_level];
          
          if (next_node && next_node->key < key) {
            current_node = next_node;
            continue;
          }
          
          update[cur_level] = current_node;
          cur_level--;
        }

        Node<K, V>* candidate = current_node->forward[0];
        
        if (candidate && candidate->key == key) {
          // update existing node
          candidate->value = value;
          candidate->seq = ++seq_;
          return;
        }

        // create new node
        int node_level = randomLevel();
        Node<K, V>* new_node = new Node(key, value, ++seq_, node_level);

        for (int level = 0; level <= node_level; level++) {
          new_node->forward[level] = update[level]->forward[level];
          update[level]->forward[level] = new_node;
        }
      }

      std::optional<V> find(const K& key) {
        Node<K, V>* node = findNode_(key);

        if (!node) return std::nullopt;

        return node->value;
      }

    private: 
      int randomLevel() {
        int lvl = 0;
        std::uniform_real_distribution<float> dist(0.0, 1.0);

        while (dist(engine_) < p_ && lvl < max_level_) lvl++;

        return lvl;
      }

      Node<K, V>* findNode_(const K& key) {
        int level = max_level_;

        Node<K, V>* current_node = head_;

        while (level >= 0 && current_node) {
          Node<K, V>* next_node = current_node->forward[level];

          if (next_node && next_node->key <= key) {
            current_node = current_node->forward[level];
            continue;
          }

          level --;
        }

        if (current_node && current_node->key == key) {
          return current_node;
        }

        return nullptr;
      } 

      Node<K, V>* head_;

      const int max_level_;
      const float p_;

      uint64_t seq_;
      mutable std::mt19937 engine_;
  };
}