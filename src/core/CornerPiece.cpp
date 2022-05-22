#include "CornerPiece.h"
#include <stdexcept>

CornerPiece::CornerPiece(const CornerPieceProxy& proxy)
    : first(proxy.first()), second(proxy.second()), third(proxy.third()) {}

CornerPiece::CornerPiece(const ConstCornerPieceProxy& proxy)
    : first(proxy.first()), second(proxy.second()), third(proxy.third()) {}

bool CornerPiece::operator==(const CornerPiece& other) const {
  return first == other.first && second == other.second && third == other.third;
}

bool CornerPiece::operator!=(const CornerPiece& other) const {
  return !(*this == other);
}

CornerPiece CornerPiece::rotateClockwise() const {
  return {second, third, first};
}

CornerPiece CornerPiece::rotateCounterclockwise() const {
  return {third, first, second};
}

CornerPiece CornerPiece::rotate(
    const CornerRotationAmount& rotation_amount) const {
  switch (rotation_amount) {
    case CornerRotationAmount::NONE:
      return *this;
    case CornerRotationAmount::CLOCKWISE:
      return rotateClockwise();
    case CornerRotationAmount::COUNTERCLOCKWISE:
      return rotateCounterclockwise();
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

CornerPieceProxy::CornerPieceProxy(CornerPiece& corner_piece,
                                   const CornerRotationAmount& rotation_amount)
    : corner_piece(corner_piece), rotation_amount(rotation_amount) {}

CornerPiece CornerPieceProxy::value() const {
  return corner_piece.rotate(rotation_amount);
}

Colour CornerPieceProxy::first() const { return value().first; }

Colour CornerPieceProxy::second() const { return value().second; }

Colour CornerPieceProxy::third() const { return value().third; }

void CornerPieceProxy::operator=(const CornerPiece& other) {
  // instead of rotating this piece, rotate the other piece in the opposite
  // direction
  corner_piece = other.rotate(-rotation_amount);
}

ConstCornerPieceProxy::ConstCornerPieceProxy(
    const CornerPiece& corner_piece,
    const CornerRotationAmount& rotation_amount)
    : corner_piece(corner_piece), rotation_amount(rotation_amount) {}

CornerPiece ConstCornerPieceProxy::value() const {
  return corner_piece.rotate(rotation_amount);
}

Colour ConstCornerPieceProxy::first() const { return value().first; }

Colour ConstCornerPieceProxy::second() const { return value().second; }

Colour ConstCornerPieceProxy::third() const { return value().third; }