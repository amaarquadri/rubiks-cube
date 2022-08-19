#pragma once

#include "CornerRotationAmount.h"
#include "Face.h"
#include <cstddef>
#include <stdexcept>

struct CornerLocation {
  // must be defined in clockwise order
  Face first;
  Face second;
  Face third;

  [[nodiscard]] constexpr bool operator==(const CornerLocation& other) const =
      default;

  [[nodiscard]] constexpr bool operator!=(const CornerLocation& other) const =
      default;

  /**
   * @return The CornerLocation that is a clockwise rotation from this
   * CornerLocation. For example, {U, R, F} becomes {R, F, U}.
   */
  [[nodiscard]] constexpr CornerLocation rotateClockwise() const {
    return {second, third, first};
  }

  /**
   * @return The CornerLocation that is a counterclockwise rotation from this
   * CornerLocation. For example, {U, R, F} becomes {F, U, R}.
   */
  [[nodiscard]] constexpr CornerLocation rotateCounterclockwise() const {
    return {third, first, second};
  }

  [[nodiscard]] constexpr CornerLocation rotate(
      const CornerRotationAmount& rotation_amount) const {
    switch (rotation_amount) {
      case CornerRotationAmount::None:
        return *this;
      case CornerRotationAmount::Clockwise:
        return rotateClockwise();
      case CornerRotationAmount::Counterclockwise:
        return rotateCounterclockwise();
      default:
        throw std::logic_error("Unknown enum value!");
    }
  }
};

namespace std {
template <>
struct hash<CornerLocation> {
  size_t operator()(const CornerLocation& cornerLocation) const {
    static constexpr std::hash<Face> hasher{};
    return 31 * 31 * hasher(cornerLocation.first) +
           31 * hasher(cornerLocation.second) + hasher(cornerLocation.third);
  }
};
}  // namespace std
