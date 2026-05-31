#include "lsm/db/compaction/compactor.h"
#include "lsm/db/sstable/sstable_builder.h"
#include "lsm/db/sstable/sstable.h"
#include "lsm/db/sstable/sstable_iterator.h"

#include <vector>

namespace lsm {
  void Compactor::compact(
    std::vector<SSTableMetadata>& input, 
    std::filesystem::path output_path
  ) {
    SSTableBuilder buidler(output_path);
 
    std::vector<SSTable> tables;
    std::vector<SSTableIterator> iterators;

    for (auto sstable_metadata: input) {
      tables.push_back(SSTable(sstable_metadata.path));
      iterators.push_back(SSTableIterator(tables.back()));
      iterators.back().first();
    }

    while (true) {
      SSTableIterator* curr_it = nullptr;

      for (SSTableIterator& it: iterators ) {
        if (it.is_done()) continue;
        if (!curr_it || it.key() < curr_it->key()) curr_it = &it;
      }
    
      if (!curr_it) break; // done
      buidler.add(curr_it->key(), curr_it->value());
      curr_it->next();
    }

    buidler.finish();
  } 
}