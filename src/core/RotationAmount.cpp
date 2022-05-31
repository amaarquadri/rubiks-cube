#include "RotationAmount.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

std::string toStr(const RotationAmount& rotationAmount) {
  switch (rotationAmount) {
    case RotationAmount::None:
    case RotationAmount::Clockwise:
      return "";
    case RotationAmount::HalfTurn:
      return "2";
    case RotationAmount::Counterclockwise:
      return "'";
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

std::pair<size_t, RotationAmount> parseRotationAmount(const std::string& str) {
  size_t consumed = 0;
  size_t rotation_amount = 1;  // default to a single Clockwise RotationAmount
  for (const char& chr : str) {
    if (chr >= '0' && chr <= '9') {
      int digit = chr - '0';
      rotation_amount =
          consumed == 0 ? digit % 4 : (2 * rotation_amount + digit) % 4;
      consumed++;
    } else if (chr == '\'') {
      rotation_amount = (4 - rotation_amount) % 4;
      consumed++;
      break;
    } else
      break;
  }

  switch (rotation_amount) {
    case 0:
      return {consumed,
              consumed == 0 ? RotationAmount::Clockwise : RotationAmount::None};
    case 1:
      return {consumed, RotationAmount::Clockwise};
    case 2:
      return {consumed, RotationAmount::HalfTurn};
    case 3:
      return {consumed, RotationAmount::Counterclockwise};
    default:
      throw std::runtime_error("Impossible result mod 4!");
  }
}
