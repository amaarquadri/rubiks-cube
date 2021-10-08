#pragma once

#include "Colour.h"

struct CornerPiece {
    Colour first;
    // must be defined in clockwise order
    Colour second;
    Colour third;

    bool operator==(const CornerPiece &other) const;

    CornerPiece rotateClockwise() const;

    CornerPiece rotateCounterclockwise() const;
};
