#pragma once

#include "Colour.h"

struct CornerPiece {
    // must be defined in clockwise order
    Colour second;
    Colour first;
    Colour third;

    bool operator==(const CornerPiece &other) const;

    [[nodiscard]] CornerPiece rotateClockwise() const;

    [[nodiscard]] CornerPiece rotateCounterclockwise() const;
};
