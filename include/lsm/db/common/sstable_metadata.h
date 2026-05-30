
#include <filesystem>

namespace lsm {
  struct SSTableMetadata {
    unsigned short level;
    std::filesystem::path path;

    std::string min_key;
    std::string max_key;
  };
}