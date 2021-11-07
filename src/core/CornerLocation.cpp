#include "CornerLocation.h"

bool CornerLocation::operator==(const CornerLocation &other) const {
    return first == other.first && second == other.second && third == other.third;
}

bool CornerLocation::operator!=(const CornerLocation &other) const {
    return !(*this == other);
}

CornerLocation CornerLocation::rotateClockwise() const {
    return {third, first, second};
}

CornerLocation CornerLocation::rotateCounterClockwise() const {
    return {second, third, first};
}
