#include "CornerPiece.h"
#include "Colour.h"
#include "CornerRotationAmount.h"
#include <stdexcept>
#include <utility>

CornerPiece::CornerPiece(const CornerPieceProxy& proxy)
    : first(proxy.first()), second(proxy.second()), third(proxy.third()) {}

CornerPiece::CornerPiece(const ConstCornerPieceProxy& proxy)
    : first(proxy.first()), second(proxy.second()), third(proxy.third()) {}

CornerPiece CornerPiece::rotateClockwise() const {
  return {second, third, first};
}

CornerPiece CornerPiece::rotateCounterclockwise() const {
  return {third, first, second};
}

CornerPiece CornerPiece::rotate(
    const CornerRotationAmount& rotation_amount) const {
  using enum CornerRotationAmount;
  switch (rotation_amount) {
    case None:
      return *this;
    case Clockwise:
      return rotateClockwise();
    case Counterclockwise:
      return rotateCounterclockwise();
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

void CornerPiece::rotateInPlace(const CornerRotationAmount& rotation_amount) {
  using enum CornerRotationAmount;
  switch (rotation_amount) {
    case None:
      break;
    case Clockwise:
      // first, second, third = second, third, first
      first = std::exchange(second, std::exchange(third, first));
      break;
    case Counterclockwise:
      // first, second, third = third, first, second
      first = std::exchange(third, std::exchange(second, first));
      break;
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

CornerPieceProxy& CornerPieceProxy::operator=(const CornerPiece& other) {
  // instead of rotating this piece, rotate the other piece in the opposite
  // direction
  corner_piece = other.rotate(-rotation_amount);
  return *this;
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