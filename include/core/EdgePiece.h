#pragma once

#include "Colour.h"
#include <algorithm>
#include <cstddef>

struct EdgePiece {
  Colour first;
  Colour second;

  constexpr EdgePiece() = default;

  constexpr EdgePiece(const Colour& first, const Colour& second)
      : first(first), second(second) {}

  constexpr bool operator==(const EdgePiece& other) const = default;

  constexpr bool operator!=(const EdgePiece& other) const = default;

  [[nodiscard]] constexpr EdgePiece flip() const { return {second, first}; }

  constexpr void flipInPlace() { std::swap(first, second); }
};

class EdgePieceProxy {
 private:
  EdgePiece& edge_piece;
  const bool is_flipped;

 public:
  constexpr EdgePieceProxy(EdgePiece& edge_piece, const bool& is_flipped)
      : edge_piece(edge_piece), is_flipped(is_flipped) {}

  constexpr operator EdgePiece() const {  // NOLINT(google-explicit-constructor)
    return is_flipped ? edge_piece.flip() : edge_piece;
  }

  [[nodiscard]] constexpr Colour first() const {
    return static_cast<EdgePiece>(*this).first;
  }

  [[nodiscard]] constexpr Colour second() const {
    return static_cast<EdgePiece>(*this).second;
  }

  constexpr EdgePieceProxy& operator=(const EdgePiece& other) {
    edge_piece = is_flipped ? other.flip() : other;
    return *this;
  }

  [[nodiscard]] constexpr EdgePiece flip() const {
    return static_cast<EdgePiece>(*this).flip();
  }

  constexpr void flipInPlace() { edge_piece.flipInPlace(); }
};

class ConstEdgePieceProxy {
 private:
  const EdgePiece& edge_piece;
  const bool is_flipped;

 public:
  constexpr ConstEdgePieceProxy(const EdgePiece& edge_piece,
                                const bool& is_flipped)
      : edge_piece(edge_piece), is_flipped(is_flipped) {}

  constexpr operator EdgePiece() const {  // NOLINT(google-explicit-constructor)
    return is_flipped ? edge_piece.flip() : edge_piece;
  }

  [[nodiscard]] constexpr Colour first() const {
    return static_cast<EdgePiece>(*this).first;
  }

  [[nodiscard]] constexpr Colour second() const {
    return static_cast<EdgePiece>(*this).second;
  }

  [[nodiscard]] constexpr EdgePiece flip() const {
    return static_cast<EdgePiece>(*this).flip();
  }
};

namespace std {
template <>
struct hash<EdgePiece> {
  size_t operator()(const EdgePiece& edge_location) const {
    static constexpr std::hash<Colour> hasher{};
    return 31 * hasher(edge_location.first) + hasher(edge_location.second);
  }
};
}  // namespace std
