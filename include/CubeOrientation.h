#pragma once

#include "Colour.h"

struct CubeOrientation {
    Colour top;
    Colour front;

    bool operator==(const CubeOrientation &other) const;

    [[nodiscard]] Colour getRightFaceColour() const;

    [[nodiscard]] Colour getBackFaceColour() const;

    [[nodiscard]] Colour getLeftFaceColour() const;

    [[nodiscard]] Colour getBottomFaceColour() const;
};
