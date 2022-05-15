#pragma once

#include <cstdint>

enum class CornerRotationAmount : uint8_t { NONE, CLOCKWISE, COUNTERCLOCKWISE };

CornerRotationAmount inv(const CornerRotationAmount& rotationAmount);

CornerRotationAmount operator+(const CornerRotationAmount& first,
                               const CornerRotationAmount& second);

CornerRotationAmount operator-(const CornerRotationAmount& first,
                               const CornerRotationAmount& second);

CornerRotationAmount operator+(const CornerRotationAmount& rotation_amount);

CornerRotationAmount operator-(const CornerRotationAmount& rotation_amount);
