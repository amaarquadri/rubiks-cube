#pragma once

#include "CornerRotationAmount.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

enum class RotationAmount : uint8_t {
  None,
  Clockwise,
  HalfTurn,
  Counterclockwise
};

std::string toStr(const RotationAmount& rotationAmount);

constexpr CornerRotationAmount toCornerRotationAmount(
    const RotationAmount& rotation_amount) {
  switch (rotation_amount) {
    case RotationAmount::None:
      return CornerRotationAmount::None;
    case RotationAmount::Clockwise:
      return CornerRotationAmount::Clockwise;
    case RotationAmount::HalfTurn:
      throw std::invalid_argument(
          "HalfTurn does not correspond to any CornerRotationAmount!");
    case RotationAmount::Counterclockwise:
      return CornerRotationAmount::Counterclockwise;
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

constexpr RotationAmount operator+(const RotationAmount& first,
                                   const RotationAmount& second) {
  const uint8_t clockwise_turns =
      (static_cast<uint8_t>(first) + static_cast<uint8_t>(second)) % 4;
  return static_cast<RotationAmount>(clockwise_turns);
}

constexpr void operator+=(RotationAmount& first, const RotationAmount& second) {
  first = first + second;
}

constexpr RotationAmount operator-(const RotationAmount& first,
                                   const RotationAmount& second) {
  const uint8_t clockwise_turns =
      (4 + static_cast<uint8_t>(first) - static_cast<uint8_t>(second)) % 4;
  return static_cast<RotationAmount>(clockwise_turns);
}

constexpr void operator-=(RotationAmount& first, const RotationAmount& second) {
  first = first - second;
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
