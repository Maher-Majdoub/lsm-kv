#include "lsm/db/db.h"

#include <cassert>
#include <cstddef>

int main() {
  lsm::DB db;

  int n = 1000;

  for (int i = 0; i < n; i++) {
    std::string key = "key" + std::to_string(i), value = "value" + std::to_string(i);
    db.set(key, value);
  }
}