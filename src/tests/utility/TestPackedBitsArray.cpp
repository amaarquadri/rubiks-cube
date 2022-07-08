#include "TestPackedBitsArray.h"
#include "PackedBitsArray.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>

void testPackedBitsArray() {
  using utility::PackedBitsArray;

  static constexpr uint32_t multiplier = (1 << 13) + (1 << 8) + 1;
  PackedBitsArray<25, 800> arr;
  static_assert(800 * multiplier < (1 << 25));
  static_assert(sizeof(arr) == 2500);
  static_assert(std::is_same_v<decltype(arr)::const_reference, uint32_t>);
  for (size_t i = 0; i < arr.size(); ++i) arr[i] = multiplier * i;
  for (size_t i = 0; i < arr.size(); ++i)
    if (arr[i] != multiplier * i)
      throw std::logic_error("PackedBitsArray failed!");

  std::cout << "Passed all tests for PackedBitsArray!\n";
}
