#pragma once

#include <cstdint>
#include <string>

enum class Colour : uint8_t { White, Green, Red, Blue, Orange, Yellow };

Colour getOpposite(const Colour& colour);

Colour getLeft(const Colour& top, const Colour& front);

Colour getRight(const Colour& top, const Colour& front);

std::string toStr(const Colour& colour);
