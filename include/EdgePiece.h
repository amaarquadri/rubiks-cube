#pragma once

#include "Colour.h"

struct EdgePiece {
    Colour first;
    Colour second;

    bool operator==(const EdgePiece &other) const;

    EdgePiece flip() const;
};
