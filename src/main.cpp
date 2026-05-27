#include <cstddef>
#include "lsm/sstable/sstable.h"
#include "lsm/sstable/sstable_builder.h"
#include<lsm/db/db.h>

int main() {
  SSTableBuilder sstable = SSTableBuilder("data.sst");

  sstable.add("1", "world1");
  sstable.add("2", "world2");
  sstable.add("3", "world3");
  sstable.add("4", "world4");
  sstable.add("5", "world5");
  sstable.finish();


  SSTable sst("data.sst");
  sst.find("4");
}