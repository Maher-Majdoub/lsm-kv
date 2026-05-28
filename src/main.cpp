#include "lsm/db/db.h"

#include <cstddef>
#include <iostream>

int main() {
  lsm::DB db;

  int n = 1000;

  for (int i = 0; i < n; i++) {
    std::string key = "key" + std::to_string(i), value = "value" + std::to_string(i);
    db.set(key, value);
  }

  auto result = db.get("key0");
  if (result) {
    std::cout << "Found: " << *result << "\n";
  }
  else {
    std::cout << "Not Found\n";
  }
}