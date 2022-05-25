#pragma once

#include "CornerRotationAmount.h"
#include "Face.h"
#include <cstddef>

struct CornerLocation {
  // must be defined in clockwise order
  Face first;
  Face second;
  Face third;

  [[nodiscard]] bool operator==(const CornerLocation& other) const;

  [[nodiscard]] bool operator!=(const CornerLocation& other) const;

  /**
   * @return The CornerLocation that is a clockwise rotation from this
   * CornerLocation. For example, {U, R, F} becomes {R, F, U}.
   */
  [[nodiscard]] CornerLocation rotateClockwise() const;

  /**
   * @return The CornerLocation that is a counterclockwise rotation from this
   * CornerLocation. For example, {U, R, F} becomes {F, U, R}.
   */
  [[nodiscard]] CornerLocation rotateCounterclockwise() const;

  [[nodiscard]] CornerLocation rotate(
      const CornerRotationAmount& rotation_amount) const;
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
