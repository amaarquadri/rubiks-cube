#pragma once

#include "Colour.h"
#include <cstddef>

class EdgePieceProxy;
class ConstEdgePieceProxy;

struct EdgePiece {
  Colour first;
  Colour second;

  constexpr EdgePiece() = default;

  constexpr EdgePiece(const Colour& first, const Colour& second)
      : first(first), second(second) {}

  EdgePiece(const EdgePieceProxy& proxy);

  EdgePiece(const ConstEdgePieceProxy& proxy);

  bool operator==(const EdgePiece& other) const;

  bool operator!=(const EdgePiece& other) const;

  [[nodiscard]] EdgePiece flip() const;

  void flipInPlace();
};

class EdgePieceProxy {
 private:
  EdgePiece& edge_piece;
  const bool is_flipped;

 public:
  EdgePieceProxy(EdgePiece& edge_piece, const bool& is_flipped);

  EdgePiece value() const;

  Colour first() const;

  Colour second() const;

  void operator=(const EdgePiece& other);

  [[nodiscard]] EdgePiece flip() const;

  void flipInPlace();
};

class ConstEdgePieceProxy {
 private:
  const EdgePiece& edge_piece;
  const bool is_flipped;

 public:
  ConstEdgePieceProxy(const EdgePiece& edge_piece, const bool& is_flipped);

  EdgePiece value() const;

  Colour first() const;

  Colour second() const;

  [[nodiscard]] EdgePiece flip() const;
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
