#include "CornerPiece.h"

bool CornerPiece::operator==(const CornerPiece& other) const {
  return first == other.first && second == other.second && third == other.third;
}

bool CornerPiece::operator!=(const CornerPiece& other) const {
  return !(*this == other);
}

CornerPiece CornerPiece::rotateClockwise() const {
  return {third, first, second};
}

CornerPiece CornerPiece::rotateCounterclockwise() const {
  return {second, third, first};
}
