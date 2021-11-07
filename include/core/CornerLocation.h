#pragma once

#include "Face.h"

struct CornerLocation {
    // must be defined in clockwise order
    Face first;
    Face second;
    Face third;

    [[nodiscard]] bool operator==(const CornerLocation &other) const;

    [[nodiscard]] bool operator!=(const CornerLocation &other) const;

    [[nodiscard]] CornerLocation rotateClockwise() const;

    [[nodiscard]] CornerLocation rotateCounterClockwise() const;
};

namespace std {
    template<>
    struct hash<CornerLocation> {
        size_t operator()(const CornerLocation &cornerLocation) const {
            return 36 * static_cast<size_t>(cornerLocation.first) + 6 * static_cast<size_t>(cornerLocation.second) +
            static_cast<size_t>(cornerLocation.third);
        }
    };
}
