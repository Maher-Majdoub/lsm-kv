#include "lsm/db/db.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

int main() {
  lsm::DB db;

  int n = 100'000;

  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < n; i++) {
    std::string key = "key" + std::to_string(i), value = "value" + std::to_string(i);
    db.set(key, value);
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> duration = end - start;
  std::cout << "Time: " << duration.count() << " seconds\n";

  // check all values exist
  for (int i = 0; i < n; i++) {
    auto key = "key" + std::to_string(i);
    auto value = db.get(key);
    
    assert(value);
  }

  std::cout << "All added values exist.\n";
  
  // check not added values deleted
  for (int i = n + 1; i < n + 20; i++) {
    auto key = "key" + std::to_string(i);
    auto value = db.get(key);
    
    assert(!value);
  }

  std::cout << "Not existing values are returing nullopt.\n";

  db.remove("key0");
  assert(!db.get("key0"));

}