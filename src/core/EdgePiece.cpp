#include "EdgePiece.h"
#include "Colour.h"
#include <algorithm>

EdgePiece::EdgePiece(const EdgePieceProxy& proxy)
    : first(proxy.first()), second(proxy.second()) {}

EdgePiece::EdgePiece(const ConstEdgePieceProxy& proxy)
    : first(proxy.first()), second(proxy.second()) {}

EdgePiece EdgePiece::flip() const { return {second, first}; }

void EdgePiece::flipInPlace() { std::swap(first, second); }

EdgePieceProxy::EdgePieceProxy(EdgePiece& edge_piece, const bool& is_flipped)
    : edge_piece(edge_piece), is_flipped(is_flipped) {}

EdgePiece EdgePieceProxy::value() const {
  return is_flipped ? edge_piece.flip() : edge_piece;
}

Colour EdgePieceProxy::first() const { return value().first; }

Colour EdgePieceProxy::second() const { return value().second; }

EdgePieceProxy& EdgePieceProxy::operator=(const EdgePiece& other) {
  if (is_flipped)
    edge_piece = other.flip();
  else
    edge_piece = other;
  return *this;
}

EdgePiece EdgePieceProxy::flip() const { return value().flip(); }

void EdgePieceProxy::flipInPlace() { edge_piece.flipInPlace(); }

ConstEdgePieceProxy::ConstEdgePieceProxy(const EdgePiece& edge_piece,
                                         const bool& is_flipped)
    : edge_piece(edge_piece), is_flipped(is_flipped) {}

EdgePiece ConstEdgePieceProxy::value() const {
  return is_flipped ? edge_piece.flip() : edge_piece;
}

Colour ConstEdgePieceProxy::first() const { return value().first; }

Colour ConstEdgePieceProxy::second() const { return value().second; }

EdgePiece ConstEdgePieceProxy::flip() const { return value().flip(); }
