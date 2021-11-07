#include "EdgePiece.h"

bool EdgePiece::operator==(const EdgePiece &other) const {
    return first == other.first && second == other.second;
}

bool EdgePiece::operator!=(const EdgePiece &other) const {
    return !(*this == other);
}

EdgePiece EdgePiece::flip() const {
    return {second, first};
}
