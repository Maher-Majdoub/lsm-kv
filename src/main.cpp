#include "lsm/db/db.h"
// #include "lsm/db/sstable/sstable.h"
// #include "lsm/db/sstable/sstable_builder.h"
// #include "lsm/db/memtable/memtable.h"
// #include "lsm/db/db.h"

#include <cstddef>
// #include <iostream>

int main() {
  // lsm::SSTableBuilder sstable("data.sst");

  // sstable.add("1", "world1");
  // sstable.add("2", "world2");
  // sstable.add("3", "world3");
  // sstable.add("4", "world4");
  // sstable.add("5", "world5");
  // sstable.finish();


  // lsm::SSTable sst("data.sst");
  // auto result = sst.find("4");

  // std::cout << (result ? ((std::string)"found : " + *result) : "not found") << "\n";


  lsm::DB db;

  db.set("key1", "value1");
  db.set("key2", "value2");
  db.set("key3", "value3");

  db.display_memtable();
}