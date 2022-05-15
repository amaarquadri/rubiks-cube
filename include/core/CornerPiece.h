#pragma once

#include "Colour.h"
#include "CornerRotationAmount.h"

struct CornerPiece {
  // must be defined in clockwise order
  Colour first;
  Colour second;
  Colour third;

  bool operator==(const CornerPiece& other) const;

  bool operator!=(const CornerPiece& other) const;

  [[nodiscard]] CornerPiece rotateClockwise() const;

  [[nodiscard]] CornerPiece rotateCounterclockwise() const;

  [[nodiscard]] CornerPiece rotate(
      const CornerRotationAmount& rotation_amount) const;
};

namespace std {
template <>
struct hash<CornerPiece> {
  size_t operator()(const CornerPiece& corner_location) const {
    return 31 * 31 * std::hash<Colour>{}(corner_location.first) +
           31 * std::hash<Colour>{}(corner_location.second) +
           std::hash<Colour>{}(corner_location.third);
  }
};
}  // namespace std
