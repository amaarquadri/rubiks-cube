#include "CornerPiece.h"
#include <stdexcept>

bool CornerPiece::operator==(const CornerPiece& other) const {
  return first == other.first && second == other.second && third == other.third;
}

bool CornerPiece::operator!=(const CornerPiece& other) const {
  return !(*this == other);
}

CornerPiece CornerPiece::rotateClockwise() const {
  return {third, first, second};
}

CornerPiece CornerPiece::rotateCounterclockwise() const {
  return {second, third, first};
}

CornerPiece CornerPiece::rotate(
    const CornerRotationAmount& rotation_amount) const {
  switch (rotation_amount) {
    case CornerRotationAmount::NONE:
      return *this;
    case CornerRotationAmount::CLOCKWISE:
      return rotateClockwise();
    case CornerRotationAmount::COUNTERCLOCKWISE:
      return rotateCounterclockwise();
    default:
      throw std::logic_error("Unknown enum value!");
  }
}
