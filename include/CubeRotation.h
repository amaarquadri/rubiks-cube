#pragma once

#include "Face.h"
#include "Turn.h"
#include "CubeOrientation.h"
#include <string>

struct CubeRotation {
    Face newTop;
    Face newFront;

    [[nodiscard]] Face apply(const Face &face) const;

    CubeRotation inv() const;

    CubeRotation operator*(const CubeRotation &other) const;

    CubeOrientation operator*(const CubeOrientation &cubeOrientation) const;

    // Returns the equivalent turn
    Turn operator*(const Turn &turn) const;

    [[nodiscard]] std::string toStr() const;
};
