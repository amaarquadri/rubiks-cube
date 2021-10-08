#pragma once

#include "Face.h"

struct CornerLocation {
    Face second;
    // must be defined in clockwise order
    Face first;
    Face third;

    bool operator==(const CornerLocation &other) const;

    [[nodiscard]] CornerLocation rotateClockwise() const;

    [[nodiscard]] CornerLocation rotateCounterClockwise() const;
};
