#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace utility {
static constexpr std::array<size_t, 8> PowersOf3{1,  3,   9,   27,
                                                 81, 243, 729, 2187};

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

template <typename T1, size_t n, std::integral T2, size_t k>
constexpr void cycleArray(std::array<T1, n>& array,
                          const std::array<T2, k>& cycle) {
  static_assert(k >= 2, "Must cycle at least 2 elements!");
  T1 next = array[cycle.front()];
  for (size_t i = 1; i < cycle.size(); ++i) std::swap(array[cycle[i]], next);
  array[cycle.front()] = next;
}

/***
 * Computes the Levenshtein edit distance
 */
template <typename T>
size_t levEditDistance(const std::vector<T>& first,
                       const std::vector<T>& second) {
  // Based on: https://youtu.be/Cu7Tl7FGigQ
  const size_t N = first.size();
  const size_t M = second.size();

  // ensure that M <= N, to reduce memory usage
  if (N < M) return levEditDistance(second, first);

  std::vector<size_t> current_row(M);
  for (size_t m = 0; m < M; ++m) current_row[m] = m;

  for (size_t n = 1; n < N; ++n) {
    size_t last_row_last_column = current_row[0];
    current_row[0] = n;
    for (size_t m = 1; m < M; ++m) {
      if (first[n] != second[m]) {
        // store the desired value of current_row[m] in last_row_last_column
        if (current_row[m - 1] < last_row_last_column)
          last_row_last_column = current_row[m - 1];
        if (current_row[m] < last_row_last_column)
          last_row_last_column = current_row[m];
        ++last_row_last_column;
      }
      std::swap(last_row_last_column, current_row[m]);
    }
  }
  return current_row[M - 1];
}
}  // namespace utility
