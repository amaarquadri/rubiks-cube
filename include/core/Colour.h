#pragma once

#include <cstdint>
#include <string>

enum class Colour : uint8_t { WHITE, GREEN, RED, BLUE, ORANGE, YELLOW };

Colour getOpposite(const Colour& colour);

Colour getLeft(const Colour& top, const Colour& front);

Colour getRight(const Colour& top, const Colour& front);

std::string toStr(const Colour& colour);
