#include <cstddef>
#include <iostream>
#include<lsm/db/db.h>

int main() {
  lsm::DB db;

  db.set("name", "maher");

  auto name = db.get("name");
  std::cout << "name: " << (name ? *name : "NOT FOUND") << "\n";
}