#pragma once

#include "Colour.h"
#include "CornerRotationAmount.h"
#include <cstddef>

class CornerPieceProxy;
class ConstCornerPieceProxy;

struct CornerPiece {
  // must be defined in clockwise order
  Colour first;
  Colour second;
  Colour third;

  constexpr CornerPiece() = default;

  constexpr CornerPiece(const Colour& first, const Colour& second,
                        const Colour& third)
      : first(first), second(second), third(third) {}

  CornerPiece(const CornerPieceProxy& proxy);

  CornerPiece(const ConstCornerPieceProxy& proxy);

  constexpr bool operator==(const CornerPiece& other) const = default;

  constexpr bool operator!=(const CornerPiece& other) const = default;

  [[nodiscard]] CornerPiece rotateClockwise() const;

  [[nodiscard]] CornerPiece rotateCounterclockwise() const;

  [[nodiscard]] CornerPiece rotate(
      const CornerRotationAmount& rotation_amount) const;
};

class CornerPieceProxy {
 private:
  CornerPiece& corner_piece;
  const CornerRotationAmount rotation_amount;

 public:
  /**
   * Represents a proxy for the corner_piece with the given applied rotation
   * (i.e. corner_piece.rotate(rotation_amount)).
   */
  CornerPieceProxy(CornerPiece& corner_piece,
                   const CornerRotationAmount& rotationAmount);

  CornerPiece value() const;

  Colour first() const;

  Colour second() const;

  Colour third() const;

  void operator=(const CornerPiece& other);
};

class ConstCornerPieceProxy {
 private:
  const CornerPiece& corner_piece;
  const CornerRotationAmount rotation_amount;

 public:
  ConstCornerPieceProxy(const CornerPiece& corner_piece,
                        const CornerRotationAmount& rotationAmount);

  CornerPiece value() const;

  Colour first() const;

  Colour second() const;

  Colour third() const;
};

namespace std {
template <>
struct hash<CornerPiece> {
  size_t operator()(const CornerPiece& corner_location) const {
    static constexpr std::hash<Colour> hasher{};
    return 31 * 31 * hasher(corner_location.first) +
           31 * hasher(corner_location.second) + hasher(corner_location.third);
  }
};
}  // namespace std
