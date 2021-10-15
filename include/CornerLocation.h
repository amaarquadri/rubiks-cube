#pragma once

#include "Face.h"

struct CornerLocation {
    // must be defined in clockwise order
    Face first;
    Face second;
    Face third;

    bool operator==(const CornerLocation &other) const;

    [[nodiscard]] CornerLocation rotateClockwise() const;

    [[nodiscard]] CornerLocation rotateCounterClockwise() const;
};
