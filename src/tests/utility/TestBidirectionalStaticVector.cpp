#include "TestBidirectionalStaticVector.h"
#include "BidirectionalStaticVector.h"
#include <cstddef>
#include <stdexcept>

void testBidirectionalStaticVector() {
  using namespace utility;

  auto [data_store, forward, backward] =
      BidirectionalStaticVector<int, 100>::make();
  for (size_t i = 0; i < 40; ++i) forward.push_back(5);
  for (size_t i = 0; i < 59; ++i) backward.push_back(6);

  if (forward.getSize() != 40 || backward.getSize() != 59 ||
      forward.isEmpty() || backward.isEmpty())
    throw std::logic_error("Incorrect sizes!");
}
