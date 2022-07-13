#include "TestPermutation.h"
#include "Permutation.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <numeric>

template <size_t n>
static constexpr bool testRankConsistency() {
  static_assert(n <= (1 << 8));
  std::array<size_t, n> tests;
  std::iota(tests.begin(), tests.end(), 0);
  return std::all_of(tests.begin(), tests.end(), [](const size_t& size) {
    return Permutation<n>::parseRank(size).getRank() == size;
  });
}

template <size_t n>
static constexpr bool testConsistencyRecursive() {
  if constexpr (n == 2)
    return testRankConsistency<2>();
  else
    return testRankConsistency<n>() && testConsistencyRecursive<n - 1>();
}

void testPermutation() {
  // test getRank
  static_assert(Permutation<2>{0, 1}.getRank() == 0);
  static_assert(Permutation<2>{1, 0}.getRank() == 1);
  static_assert(Permutation<3>{0, 1, 2}.getRank() == 0);
  static_assert(Permutation<3>{0, 2, 1}.getRank() == 1);
  static_assert(Permutation<3>{1, 0, 2}.getRank() == 2);
  static_assert(Permutation<3>{1, 2, 0}.getRank() == 3);
  static_assert(Permutation<3>{2, 0, 1}.getRank() == 4);
  static_assert(Permutation<3>{2, 1, 0}.getRank() == 5);
  static_assert(Permutation<4>{3, 2, 1, 0}.getRank() == 23);

  static_assert(testConsistencyRecursive<12>());
  std::cout << "Passed all tests for Permutation!\n";
}
