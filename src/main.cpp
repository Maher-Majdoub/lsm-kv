#include<iostream>
#include<string>
#include<lsm/memtable/memtable.h>

int main() {
  lsm::MemTable table;

  std::string operation, key, value;

  while (true) {
    std::cin >> operation >> key;

    if (operation == "set" || operation == "update") {
      std::cin >> value;

      table.set(key, value);
    } 
    else if (operation == "delete") {
      table.remove(key);
    } 
    else if (operation == "get") {
      auto result = table.get(key);
      std::cout << (result ? *result : "NOT FOUND") <<  std::endl;
    } 
    else {
      std::cout << "END" << std::endl;
      break;
    }
  }
}