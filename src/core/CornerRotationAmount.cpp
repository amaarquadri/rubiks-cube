#include "CornerRotationAmount.h"
#include <stdexcept>

CornerRotationAmount inv(const CornerRotationAmount& rotationAmount) {
  switch (rotationAmount) {
    case NONE:
      return NONE;
    case CLOCKWISE:
      return COUNTERCLOCKWISE;
    case COUNTERCLOCKWISE:
      return CLOCKWISE;
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

CornerRotationAmount operator+(const CornerRotationAmount& first,
                               const CornerRotationAmount& second) {
  uint8_t clockwiseTurns =
      (static_cast<uint8_t>(first) + static_cast<uint8_t>(second)) % 3;
  return static_cast<CornerRotationAmount>(clockwiseTurns);
}
