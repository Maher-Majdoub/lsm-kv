#include "lsm/db/db.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

int main() {
  lsm::DB db;

  // int n = 1000;

  // for (int i = 0; i < n; i++) {
  //   std::string key = "key" + std::to_string(i), value = "value" + std::to_string(i);
  //   db.set(key, value);
  // }

  // // check all values exist
  // for (int i = 0; i < n - 500; i++) {
  //   auto key = "key" + std::to_string(i);
  //   auto value = db.get(key);
    
  //   assert(value);
  // }

  // std::cout << "All added values exist.\n";
  
  // // check not added values deleted
  // for (int i = n + 1; i < n + 20; i++) {
  //   auto key = "key" + std::to_string(i);
  //   auto value = db.get(key);
    
  //   assert(!value);
  // }

  // std::cout << "Not existing values are returing nullopt.\n";
}