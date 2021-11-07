#pragma once

#include "Colour.h"

struct EdgePiece {
    Colour first;
    Colour second;

    bool operator==(const EdgePiece &other) const;

    bool operator!=(const EdgePiece &other) const;

    [[nodiscard]] EdgePiece flip() const;
};

namespace std {
    template<>
    struct hash<EdgePiece> {
        size_t operator()(const EdgePiece &edge_location) const {
            return 31 * std::hash<Colour>{}(edge_location.first) + std::hash<Colour>{}(edge_location.second);
        }
    };
}

