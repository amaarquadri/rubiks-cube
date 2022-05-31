#pragma once

#include <cstdint>

enum class CornerRotationAmount : uint8_t { NONE, CLOCKWISE, COUNTERCLOCKWISE };

constexpr CornerRotationAmount operator+(const CornerRotationAmount& first,
                                         const CornerRotationAmount& second);

constexpr CornerRotationAmount operator-(const CornerRotationAmount& first,
                                         const CornerRotationAmount& second);

constexpr CornerRotationAmount operator+(
    const CornerRotationAmount& rotation_amount);

constexpr CornerRotationAmount operator-(
    const CornerRotationAmount& rotation_amount);
