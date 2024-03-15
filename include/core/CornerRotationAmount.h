#pragma once

#include <cstdint>

enum class CornerRotationAmount : uint8_t { None, Clockwise, Counterclockwise };

constexpr CornerRotationAmount operator+(const CornerRotationAmount& first,
                                         const CornerRotationAmount& second) {
  const uint8_t clockwise_turns =
      (static_cast<uint8_t>(first) + static_cast<uint8_t>(second)) % 3;
  return static_cast<CornerRotationAmount>(clockwise_turns);
}

constexpr void operator+=(CornerRotationAmount& first,
                          const CornerRotationAmount& second) {
  first = first + second;
}

constexpr CornerRotationAmount operator-(const CornerRotationAmount& first,
                                         const CornerRotationAmount& second) {
  // add 3 to ensure the result is non-negative before taking the modulus
  const uint8_t clockwise_turns =
      (3 + static_cast<uint8_t>(first) - static_cast<uint8_t>(second)) % 3;
  return static_cast<CornerRotationAmount>(clockwise_turns);
}

constexpr void operator-=(CornerRotationAmount& first,
                          const CornerRotationAmount& second) {
  first = first - second;
}

constexpr CornerRotationAmount operator+(
    const CornerRotationAmount& rotation_amount) {
  return rotation_amount;
}

constexpr CornerRotationAmount operator-(
    const CornerRotationAmount& rotation_amount) {
  const uint8_t clockwise_turns =
      (3 - static_cast<uint8_t>(rotation_amount)) % 3;
  return static_cast<CornerRotationAmount>(clockwise_turns);
}
