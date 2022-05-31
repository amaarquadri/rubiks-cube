#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

enum class RotationAmount : uint8_t {
  None,
  Clockwise,
  HalfTurn,
  Counterclockwise
};

std::string toStr(const RotationAmount& rotationAmount);

constexpr RotationAmount operator+(const RotationAmount& first,
                                   const RotationAmount& second) {
  const uint8_t clockwise_turns =
      (static_cast<uint8_t>(first) + static_cast<uint8_t>(second)) % 4;
  return static_cast<RotationAmount>(clockwise_turns);
}

constexpr RotationAmount operator-(const RotationAmount& first,
                                   const RotationAmount& second) {
  const uint8_t clockwise_turns =
      (4 + static_cast<uint8_t>(first) - static_cast<uint8_t>(second)) % 4;
  return static_cast<RotationAmount>(clockwise_turns);
}

constexpr RotationAmount operator+(const RotationAmount& rotation_amount) {
  return rotation_amount;
}

constexpr RotationAmount operator-(const RotationAmount& rotation_amount) {
  const uint8_t clockwise_turns =
      (4 - static_cast<uint8_t>(rotation_amount)) % 4;
  return static_cast<RotationAmount>(clockwise_turns);
}

/**
 * @brief Returns the number of characters consumed and the parsed
 * RotationAmount. If it was not possible to parse a RotationAmount, then the
 * number of characters consumed will be zero and the RotationAmount will be
 * Clockwise.
 */
// TODO: use size_t instead of int for number of characters consumed throughout
//  the codebase
std::pair<size_t, RotationAmount> parseRotationAmount(const std::string& str);
