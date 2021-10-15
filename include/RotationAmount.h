#pragma once

#include <cstdint>
#include <string>

enum RotationAmount : uint8_t {
    NONE, CLOCKWISE, HALF_TURN, COUNTERCLOCKWISE
};

std::string toStr(const RotationAmount &rotationAmount);

RotationAmount operator+(const RotationAmount &first, const RotationAmount &second);
