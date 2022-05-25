#include "CornerLocation.h"
#include "CornerRotationAmount.h"
#include <stdexcept>

bool CornerLocation::operator==(const CornerLocation& other) const {
  return first == other.first && second == other.second && third == other.third;
}

bool CornerLocation::operator!=(const CornerLocation& other) const {
  return !(*this == other);
}

CornerLocation CornerLocation::rotateClockwise() const {
  return {second, third, first};
}

CornerLocation CornerLocation::rotateCounterclockwise() const {
  return {third, first, second};
}

CornerLocation CornerLocation::rotate(
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
