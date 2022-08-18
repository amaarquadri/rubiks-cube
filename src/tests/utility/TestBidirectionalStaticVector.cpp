#include "TestBidirectionalStaticVector.h"
#include "BidirectionalStaticVector.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>

template <bool use_heap>
static void testForUseHeap() {
  using namespace utility;
  auto [forward, backward] =
      BidirectionalStaticVector<int, 100, use_heap>::make();
  for (size_t i = 0; i < 40; ++i) forward.push_back(5);
  for (size_t i = 0; i < 59; ++i) backward.push_back(6);

  if (forward.getSize() != 40 || backward.getSize() != 59 ||
      forward.isEmpty() || backward.isEmpty())
    throw std::logic_error("Incorrect sizes!");
}

void testBidirectionalStaticVector() {
  testForUseHeap<false>();
  testForUseHeap<true>();
  // TODO: add tests to confirm that HeapArray works in a consteval context

  std::cout << "Passed all tests for BidirectionalStaticVector!\n";
}
