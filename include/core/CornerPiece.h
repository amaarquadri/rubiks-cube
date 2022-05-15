#pragma once

#include "Colour.h"
#include "CornerRotationAmount.h"

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

  bool operator==(const CornerPiece& other) const;

  bool operator!=(const CornerPiece& other) const;

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
    return 31 * 31 * std::hash<Colour>{}(corner_location.first) +
           31 * std::hash<Colour>{}(corner_location.second) +
           std::hash<Colour>{}(corner_location.third);
  }
};
}  // namespace std
