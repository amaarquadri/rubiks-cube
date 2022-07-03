#include "TestUint24Array.h"
#include "Uint24Array.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>

void testUint24Array() {
  using utility::Uint24Array;

  static constexpr uint32_t multiplier = (1 << 16) + (1 << 8) + 1;
  Uint24Array<10> arr;
  for (size_t i = 0; i < arr.size(); ++i) arr[i] = multiplier * i;
  for (size_t i = 0; i < arr.size(); ++i)
    if (arr[i] != multiplier * i)
      throw std::logic_error("Uint24Array failed!");

  std::cout << "Passed all tests for Uint24Array!\n";
}
