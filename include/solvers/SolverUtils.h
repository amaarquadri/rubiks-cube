#pragma once

#include "Face.h"
#include <array>
#include <cstdint>
#include <stdexcept>

namespace solvers {
constexpr std::array<uint8_t, 4> getEdgeCycle(const Face& face) {
  using enum Face;
  switch (face) {
    case U:
      return {0, 1, 2, 3};
    case F:
      return {2, 4, 8, 7};
    case R:
      return {1, 5, 9, 4};
    case B:
      return {0, 6, 10, 5};
    case L:
      return {3, 7, 11, 6};
    case D:
      return {8, 9, 10, 11};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

/**
 * For the F, R, B, and L faces, the corners will also need to be rotated prior
 * to being cycled if the RotationAmount is Clockwise or Counterclockwise. In
 * this case, index 0 and 2 in the resulting array will be rotated Clockwise,
 * and index 1 and 3 will be rotated Counterclockwise.
 */
constexpr std::array<uint8_t, 4> getCornerCycle(const Face& face) {
  using enum Face;
  switch (face) {
    case U:
      return {0, 1, 2, 3};
    case F:
      return {3, 2, 5, 4};
    case R:
      return {2, 1, 6, 5};
    case B:
      return {1, 0, 7, 6};
    case L:
      return {0, 3, 4, 7};
    case D:
      return {4, 5, 6, 7};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}
}  // namespace solvers
