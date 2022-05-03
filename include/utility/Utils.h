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

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T>>& deep_vec) {
  std::vector<T> flattened;
  for (const auto& vec : deep_vec)
    for (const auto& v : vec) flattened.push_back(v);
  return flattened;
}
}  // namespace utility
