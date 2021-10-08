#pragma once

#include <cstdint>
#include <string>

enum Face : uint8_t {
    U, F, R, B, L, D
};

Face getOpposite(const Face &face);

Face getLeft(const Face &top, const Face &front);

Face getRight(const Face &top, const Face &right);

std::string toStr(const Face &face);
