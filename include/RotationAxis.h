#pragma once

#include <cstdint>

enum RotationAxis : uint8_t {
    X, Y, Z
};

std::string toStr(const RotationAxis &rotationAxis);

// returns the number of characters consumed and the parsed RotationAxis
// if it was not possible to parse a RotationAxis, then the number of characters consumed will be zero
std::pair<int, RotationAxis> parseRotationAxis(const std::string &str);
