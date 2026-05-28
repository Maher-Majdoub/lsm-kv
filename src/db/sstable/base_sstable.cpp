#include "lsm/db/sstable/base_sstable.h"

#include <string>

namespace lsm {
  BaseSStable::BaseSStable(const std::string& file_path): file_path_(file_path) {}
}
