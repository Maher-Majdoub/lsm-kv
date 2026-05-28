#pragma once

#include "skiplist.h"
#include "lsm/utils/size.h"

#include <cstddef>
#include <vector>
#include <random>

namespace lsm {
  template <typename K, typename V> 
  SkipList<K, V>::SkipList(int max_level, float p)
    : max_level_(max_level), p_(p), seq_(0) {
    head_ = new Node(K{}, V{}, 0, max_level_);
    engine_.seed(std::random_device{}());
  }
  
  template <typename K, typename V> 
  SkipList<K, V>::~SkipList() {
    Node* node = head_;
    while (node) {
      Node* next = node->forward[0];
      delete node;
      node = next;
    }
  }
  
  template <typename K, typename V> 
  void SkipList<K, V>::insert(const K& key, const V& value) {
    std::vector<Node*> update(max_level_ + 1, nullptr);
  
    Node* current_node = head_;
    int cur_level = max_level_;
  
    while (cur_level >= 0) {
      Node* next_node = current_node->forward[cur_level];
      
      if (next_node && next_node->key < key) {
        current_node = next_node;
        continue;
      }
      
      update[cur_level] = current_node;
      cur_level--;
    }
  
    Node* candidate = current_node->forward[0];
    
    if (candidate && candidate->key == key) {
      // update existing node
      bytes_ += sizeOf(value) - sizeOf(candidate->value);
      candidate->value = value;
      candidate->seq = ++seq_;
      return;
    }
  
    // create new node
    int node_level = randomLevel();

    Node* new_node = new Node(key, value, ++seq_, node_level);
  
    for (int level = 0; level <= node_level; level++) {
      new_node->forward[level] = update[level]->forward[level];
      update[level]->forward[level] = new_node;
    }

    bytes_ += sizeof(Node)
            + sizeof(Node*) * (node_level + 1)
            + lsm::sizeOf(key)
            + lsm::sizeOf(value);
  }
  
  template <typename K, typename V>
  std::optional<V> SkipList<K, V>::find(const K& key) {
      Node* node = findNode_(key);
      if (!node) return std::nullopt;
      return node->value;
  }
  
  
  template <typename K, typename V> 
  int SkipList<K, V>::randomLevel() {
    int lvl = 0;
    std::uniform_real_distribution<float> dist(0.0, 1.0);
  
    while (dist(engine_) < p_ && lvl < max_level_) lvl++;
  
    return lvl;
  }
  
  template <typename K, typename V> 
  SkipList<K, V>::Node* SkipList<K, V>::findNode_(const K& key) {
    int level = max_level_;
  
    Node* current_node = head_;
  
    while (level >= 0 && current_node) {
      Node* next_node = current_node->forward[level];
  
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

  template <typename K, typename V>
  size_t SkipList<K, V>::size() { return bytes_; }
}

