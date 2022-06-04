#pragma once

#include "Face.h"
#include <array>
#include <cstdint>
#include <stdexcept>

namespace solvers {
constexpr std::array<uint8_t, 4> getEdgeCycle(const Face& face) {
  switch (face) {
    case Face::U:
      return {0, 1, 2, 3};
    case Face::F:
      return {2, 4, 8, 7};
    case Face::R:
      return {1, 5, 9, 4};
    case Face::B:
      return {0, 6, 10, 5};
    case Face::L:
      return {3, 7, 11, 6};
    case Face::D:
      return {8, 9, 10, 11};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}
}  // namespace solvers