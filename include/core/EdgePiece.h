#pragma once

#include "Colour.h"

struct EdgePiece {
    Colour first;
    Colour second;

    bool operator==(const EdgePiece &other) const;

    bool operator!=(const EdgePiece &other) const;

    [[nodiscard]] EdgePiece flip() const;
};
