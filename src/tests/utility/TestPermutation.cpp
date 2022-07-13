#include "TestPermutation.h"
#include "MathUtils.h"
#include "Permutation.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <numeric>

template <size_t n>
static constexpr bool testRankConsistency() {
  static_assert(n <= (1 << 8));
  std::array<size_t, utility::factorial(n)> tests;
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

template <size_t n>
static constexpr auto getParities() {
  std::array<bool, utility::factorial(n)> parities;
  for (size_t i = 0; i < parities.size(); ++i)
    parities[i] = Permutation<n>::parseRank(i).isOdd();
  return parities;
}

template <size_t n>
static constexpr auto getParitiesRecursive() {
  if constexpr (n == 2)
    return std::array<bool, 2>{false, true};
  else {
    const auto last = getParitiesRecursive<n - 1>();
    std::array<bool, n * last.size()> result;

    /**
     * The result is n sequential copies of last, where the odd indexed copies
     * are inverted.
     * Thus by induction, for any value of n the result will consist of a
     * sequence of (false, true) and (true, false) in some order.
     */
    for (size_t i = 0; i < n; ++i)
      for (size_t j = 0; j < last.size(); ++j)
        result[last.size() * i + j] = (i % 2 == 0) == last[j];
    return result;
  }
}

template <size_t n>
static constexpr bool testParitiesRecursive() {
  /**
   * Since this function returns true for all tested values of n, we have an
   * empirical proof that getParities and getParitiesRecursive are equivalent
   * functions.
   * Thus, the k-th even permutation is either the 2k-th or 2k+1-th
   * permutation and the k-th odd permutation is the other one. Determining
   * which one is which is not trivial.
   */
  if constexpr (n == 2)
    return getParities<2>() == getParitiesRecursive<2>();
  else
    return getParities<n>() == getParitiesRecursive<n>() &&
           testParitiesRecursive<n - 1>();
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

  static_assert(testConsistencyRecursive<6>());
  static_assert(testParitiesRecursive<6>());

  std::cout << "Passed all tests for Permutation!\n";
}
