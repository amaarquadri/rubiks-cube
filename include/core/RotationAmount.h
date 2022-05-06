#pragma once

#include <cstdint>
#include <string>
#include <utility>

enum class RotationAmount : uint8_t { NONE, CLOCKWISE, HALF_TURN, COUNTERCLOCKWISE };

RotationAmount inv(const RotationAmount& rotationAmount);

std::string toStr(const RotationAmount& rotationAmount);

RotationAmount operator+(const RotationAmount& first,
                         const RotationAmount& second);

/**
 * @brief Returns the number of characters consumed and the parsed
 * RotationAmount. If it was not possible to parse a RotationAmount, then the
 * number of characters consumed will be zero and the RotationAmount will be
 * CLOCKWISE
 */
std::pair<int, RotationAmount> parseRotationAmount(const std::string& str);
