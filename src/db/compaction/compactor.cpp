#include "lsm/db/compaction/compactor.h"
#include "lsm/db/common/sstable_metadata.h"
#include "lsm/db/sstable/sstable_builder.h"
#include "lsm/db/sstable/sstable_iterator.h"
#include "lsm/db/sstable/sstable.h"

#include <vector>

namespace lsm {
  std::pair<std::string, std::string> Compactor::compact(
    std::vector<SSTableMetadata>& input, 
    std::filesystem::path output_path
  ) {
    SSTableBuilder buidler(output_path);
    
    std::string min_key = "", max_key = "";
 
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

      // TODO: Implement more efficient way to handle this
      std::string key = curr_it->key();
      min_key = !min_key.size() ? key : min(key, min_key);
      max_key = !max_key.size() ? key : max(key, max_key);

      curr_it->next();
    }

    buidler.finish();


    return { min_key, max_key };
  } 
}