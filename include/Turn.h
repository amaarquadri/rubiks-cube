#pragma once

#include "Face.h"
#include "RotationAmount.h"
#include <string>
#include <utility>

struct Turn {
    Face face;
    RotationAmount rotationAmount;

    [[nodiscard]] std::string toStr() const;

    // returns the number of characters consumed and the parsed Turn
    // if it was not possible to parse a RotationAmount, then the number of characters consumed will be zero
    static std::pair<int, Turn> parse(const std::string &str);
};
