#include "RotationAmount.h"
#include <stdexcept>

std::string toStr(const RotationAmount& rotationAmount) {
  switch (rotationAmount) {
    case RotationAmount::NONE:
    case RotationAmount::CLOCKWISE:
      return "";
    case RotationAmount::HALF_TURN:
      return "2";
    case RotationAmount::COUNTERCLOCKWISE:
      return "'";
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

RotationAmount operator+(const RotationAmount& first,
                         const RotationAmount& second) {
  const uint8_t clockwise_turns =
      (static_cast<uint8_t>(first) + static_cast<uint8_t>(second)) % 4;
  return static_cast<RotationAmount>(clockwise_turns);
}

RotationAmount operator-(const RotationAmount& first,
                         const RotationAmount& second) {
  const uint8_t clockwise_turns =
      (4 + static_cast<uint8_t>(first) - static_cast<uint8_t>(second)) % 4;
  return static_cast<RotationAmount>(clockwise_turns);
}

RotationAmount operator+(const RotationAmount& rotation_amount) {
  return rotation_amount;
}

RotationAmount operator-(const RotationAmount& rotation_amount) {
  const uint8_t clockwise_turns =
      (4 - static_cast<uint8_t>(rotation_amount)) % 4;
  return static_cast<RotationAmount>(clockwise_turns);
}

std::pair<int, RotationAmount> parseRotationAmount(const std::string& str) {
  int rotationAmount = 1;  // default to a single CLOCKWISE RotationAmount
  int consumed = 0;
  for (const char& chr : str) {
    if (chr >= '0' && chr <= '9') {
      int digit = chr - '0';
      rotationAmount =
          consumed == 0 ? digit % 4 : (2 * rotationAmount + digit) % 4;
      consumed++;
    } else if (chr == '\'') {
      rotationAmount = (4 - rotationAmount) % 4;
      consumed++;
      break;
    } else
      break;
  }

  switch (rotationAmount) {
    case 0:
      return {consumed,
              consumed == 0 ? RotationAmount::CLOCKWISE : RotationAmount::NONE};
    case 1:
      return {consumed, RotationAmount::CLOCKWISE};
    case 2:
      return {consumed, RotationAmount::HALF_TURN};
    case 3:
      return {consumed, RotationAmount::COUNTERCLOCKWISE};
    default:
      throw std::runtime_error("Impossible result mod 4!");
  }
}
