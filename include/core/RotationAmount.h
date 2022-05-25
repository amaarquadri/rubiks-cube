#pragma once

#include <cstdint>
#include <string>
#include <utility>

enum class RotationAmount : uint8_t {
  // TODO: fix naming convention
  NONE,
  CLOCKWISE,
  HALF_TURN,
  COUNTERCLOCKWISE
};

std::string toStr(const RotationAmount& rotationAmount);

RotationAmount operator+(const RotationAmount& first,
                         const RotationAmount& second);

RotationAmount operator-(const RotationAmount& first,
                         const RotationAmount& second);

RotationAmount operator+(const RotationAmount& rotation_amount);

RotationAmount operator-(const RotationAmount& rotation_amount);

/**
 * @brief Returns the number of characters consumed and the parsed
 * RotationAmount. If it was not possible to parse a RotationAmount, then the
 * number of characters consumed will be zero and the RotationAmount will be
 * CLOCKWISE
 */
// TODO: use size_t instead of int for number of characters consumed throughout
//  the codebase
std::pair<int, RotationAmount> parseRotationAmount(const std::string& str);
