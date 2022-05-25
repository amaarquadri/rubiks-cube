#include "CornerRotationAmount.h"

CornerRotationAmount operator+(const CornerRotationAmount& first,
                               const CornerRotationAmount& second) {
  const uint8_t clockwiseTurns =
      (static_cast<uint8_t>(first) + static_cast<uint8_t>(second)) % 3;
  return static_cast<CornerRotationAmount>(clockwiseTurns);
}

CornerRotationAmount operator-(const CornerRotationAmount& first,
                               const CornerRotationAmount& second) {
  // add 3 to ensure the result is non-negative before taking the modulus
  const uint8_t clockwiseTurns =
      (3 + static_cast<uint8_t>(first) - static_cast<uint8_t>(second)) % 3;
  return static_cast<CornerRotationAmount>(clockwiseTurns);
}

CornerRotationAmount operator+(const CornerRotationAmount& rotation_amount) {
  return rotation_amount;
}

CornerRotationAmount operator-(const CornerRotationAmount& rotation_amount) {
  const uint8_t clockwiseTurns =
      (3 - static_cast<uint8_t>(rotation_amount)) % 3;
  return static_cast<CornerRotationAmount>(clockwiseTurns);
}