#pragma once

#include <cstddef>

namespace lsm {
  template <typename T>
  size_byte_t sizeOf(const T& val) {
    if constexpr (requires { val.size(); }) {
      return sizeof(T) + val.size();
    }

    return sizeof(T);
  }
}