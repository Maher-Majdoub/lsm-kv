#include <chrono>
#include <string>

namespace lsm {
  inline std::string current_timestamp() {
    const auto now = std::chrono::system_clock::now();
    return std::to_string(now.time_since_epoch().count());
  }
}