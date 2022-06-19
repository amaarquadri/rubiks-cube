#include "TestCombination.h"
#include "Combination.h"
#include "RandomUtils.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>

void testCombination() {
  static constexpr size_t count = 1000;
  for (size_t i = 0; i < count; ++i) {
    const size_t rank = utility::randomInt<utility::nChooseK(12, 4)>();
    if (Combination<12, 4>::parseRank(rank).getRank() != rank)
      throw std::logic_error("Inconsistent lexicographical rank!");
  }
  for (size_t i = 0; i < count; ++i) {
    const size_t rank = utility::randomInt<utility::nChooseK(8, 4)>();
    if (Combination<8, 4>::parseRank(rank).getRank() != rank)
      throw std::logic_error("Inconsistent lexicographical rank!");
  }
  std::cout << "Passed all tests for Combination!\n";
}
