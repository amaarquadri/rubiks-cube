#pragma once

#include "Face.h"
#include "RotationAmount.h"
#include <string>

struct Turn {
    Face face;
    RotationAmount rotationAmount;

    [[nodiscard]] std::string toStr() const;
};
