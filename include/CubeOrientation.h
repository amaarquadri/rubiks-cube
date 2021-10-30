#pragma once

#include "Colour.h"
#include "CubeRotation.h"

struct CubeOrientation {
    Colour top;
    Colour front;

    bool operator==(const CubeOrientation &other) const;

    [[nodiscard]] Colour getRightFaceColour() const;

    [[nodiscard]] Colour getBackFaceColour() const;

    [[nodiscard]] Colour getLeftFaceColour() const;

    [[nodiscard]] Colour getBottomFaceColour() const;

    /**
     * @brief Applies the given CubeRotation to this CubeOrientation and returns the resulting CubeOrientation
     */
    [[nodiscard]] CubeOrientation operator*(const CubeRotation &cubeRotation);
};
