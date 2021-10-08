#pragma once

#include <cstdint>
#include <string>

enum RotationAmount : uint8_t {
    CLOCKWISE, COUNTERCLOCKWISE, HALF_TURN
};

std::string toStr(const RotationAmount &rotationAmount);
