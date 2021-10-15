#pragma once

#include "Face.h"
#include "Turn.h"
#include "CubeOrientation.h"
#include <string>

struct CubeRotation {
    Face newTop; // the face that will end up on top after the rotation
    Face newFront; // the face that will end up in the front after the rotation

    [[nodiscard]] Face apply(const Face &face) const;

    [[nodiscard]] CubeRotation inv() const;

    static CubeRotation identity();

    CubeRotation operator*(const CubeRotation &other) const;

    CubeOrientation operator*(const CubeOrientation &cubeOrientation) const;

    // Returns the equivalent turn
    Turn operator*(const Turn &turn) const;

    [[nodiscard]] std::string toStr() const;
};
