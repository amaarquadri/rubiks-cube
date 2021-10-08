#pragma once

#include <cstdint>

enum Colour : uint8_t {
    WHITE, GREEN, RED, BLUE, ORANGE, YELLOW
};

Colour getOpposite(const Colour &colour);
