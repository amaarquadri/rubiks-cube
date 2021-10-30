#pragma once

#include <cstdint>

enum Slice : uint8_t {
    M, E, S
};

std::string toStr(const Slice &face);

// returns the number of characters consumed and the parsed Slice
// if it was not possible to parse a Slice, then the number of characters consumed will be zero
std::pair<int, Slice> parseSlice(const std::string &str);
