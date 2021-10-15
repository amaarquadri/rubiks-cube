#pragma once

#include <cstdint>
#include <string>

enum Colour : uint8_t {
    WHITE, GREEN, RED, BLUE, ORANGE, YELLOW
};

Colour getOpposite(const Colour &colour);

std::string toStr(const Colour &colour);
