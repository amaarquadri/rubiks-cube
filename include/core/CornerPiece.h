#pragma once

#include "Colour.h"
#include "CornerRotationAmount.h"
#include <cstddef>
#include <stdexcept>
#include <utility>

struct CornerPiece {
  // must be defined in clockwise order
  Colour first;
  Colour second;
  Colour third;

  constexpr CornerPiece() = default;

  constexpr CornerPiece(const Colour& first, const Colour& second,
                        const Colour& third)
      : first(first), second(second), third(third) {}

  constexpr bool operator==(const CornerPiece& other) const = default;

  constexpr bool operator!=(const CornerPiece& other) const = default;

  [[nodiscard]] constexpr CornerPiece rotateClockwise() const {
    return {second, third, first};
  }

  [[nodiscard]] constexpr CornerPiece rotateCounterclockwise() const {
    return {third, first, second};
  }

  [[nodiscard]] constexpr CornerPiece rotate(
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

  constexpr void rotateClockwiseInPlace() {
    // first, second, third = second, third, first
    first = std::exchange(second, std::exchange(third, first));
  }

  constexpr void rotateCounterclockwiseInPlace() {
    // first, second, third = third, first, second
    first = std::exchange(third, std::exchange(second, first));
  }

  constexpr void rotateInPlace(const CornerRotationAmount& rotation_amount) {
    using enum CornerRotationAmount;
    switch (rotation_amount) {
      case None:
        break;
      case Clockwise:
        rotateClockwiseInPlace();
        break;
      case Counterclockwise:
        rotateCounterclockwiseInPlace();
        break;
      default:
        throw std::logic_error("Unknown enum value!");
    }
  }
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
  constexpr CornerPieceProxy(CornerPiece& corner_piece,
                             const CornerRotationAmount& rotation_amount)
      : corner_piece(corner_piece), rotation_amount(rotation_amount) {}

  constexpr operator CornerPiece()  // NOLINT(google-explicit-constructor)
      const {
    return corner_piece.rotate(rotation_amount);
  }

  [[nodiscard]] constexpr Colour first() const {
    return static_cast<CornerPiece>(*this).first;
  }

  [[nodiscard]] constexpr Colour second() const {
    return static_cast<CornerPiece>(*this).second;
  }

  [[nodiscard]] constexpr Colour third() const {
    return static_cast<CornerPiece>(*this).third;
  }

  constexpr CornerPieceProxy& operator=(const CornerPiece& other) {
    // instead of rotating this piece, rotate the other piece in the opposite
    // direction
    corner_piece = other.rotate(-rotation_amount);
    return *this;
  }

  [[nodiscard]] constexpr CornerPiece rotateClockwise() const {
    return static_cast<CornerPiece>(*this).rotateClockwise();
  }

  [[nodiscard]] constexpr CornerPiece rotateCounterclockwise() const {
    return static_cast<CornerPiece>(*this).rotateCounterclockwise();
  }

  [[nodiscard]] constexpr CornerPiece rotate(
      const CornerRotationAmount& amount) const {
    return static_cast<CornerPiece>(*this).rotate(amount);
  }

  constexpr void rotateClockwiseInPlace() {
    corner_piece.rotateClockwiseInPlace();
  }

  constexpr void rotateCounterclockwiseInPlace() {
    corner_piece.rotateCounterclockwiseInPlace();
  }

  constexpr void rotateInPlace(const CornerRotationAmount& amount) {
    corner_piece.rotateInPlace(amount);
  }
};

class ConstCornerPieceProxy {
 private:
  const CornerPiece& corner_piece;
  const CornerRotationAmount rotation_amount;

 public:
  constexpr ConstCornerPieceProxy(const CornerPiece& corner_piece,
                                  const CornerRotationAmount& rotation_amount)
      : corner_piece(corner_piece), rotation_amount(rotation_amount) {}

  constexpr operator CornerPiece()  // NOLINT(google-explicit-constructor)
      const {
    return corner_piece.rotate(rotation_amount);
  }

  [[nodiscard]] constexpr Colour first() const {
    return static_cast<CornerPiece>(*this).first;
  }

  [[nodiscard]] constexpr Colour second() const {
    return static_cast<CornerPiece>(*this).second;
  }

  [[nodiscard]] constexpr Colour third() const {
    return static_cast<CornerPiece>(*this).third;
  }

  [[nodiscard]] constexpr CornerPiece rotateClockwise() const {
    return static_cast<CornerPiece>(*this).rotateClockwise();
  }

  [[nodiscard]] constexpr CornerPiece rotateCounterclockwise() const {
    return static_cast<CornerPiece>(*this).rotateCounterclockwise();
  }

  [[nodiscard]] constexpr CornerPiece rotate(
      const CornerRotationAmount& amount) const {
    return static_cast<CornerPiece>(*this).rotate(amount);
  }
};

namespace std {
template <>
struct hash<CornerPiece> {
  size_t operator()(const CornerPiece& corner_piece) const {
    static constexpr std::hash<Colour> hasher{};
    return 31 * 31 * hasher(corner_piece.first) +
           31 * hasher(corner_piece.second) + hasher(corner_piece.third);
  }
};
}  // namespace std
