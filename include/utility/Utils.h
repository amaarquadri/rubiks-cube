#pragma once

#include <cstddef>
#include <vector>

namespace utility {
template <typename T>
size_t deepCount(const std::vector<std::vector<T>>& vec) {
  size_t total = 0;
  for (const auto& v : vec) total += v.size();
  return total;
}
}  // namespace utility
