#pragma once

#include "Algorithm.h"
#include "CornerLocation.h"
#include "CornerPiece.h"
#include "CubeOrientation.h"
#include "CubeRotation.h"
#include "EdgeLocation.h"
#include "EdgePiece.h"
#include "Move.h"
#include "SliceTurn.h"
#include "Turn.h"
#include "WideTurn.h"
#include <array>
#include <cstddef>
#include <string>
#include <vector>

class Cube {
 public:
  friend struct std::hash<Cube>;

  static constexpr std::array<EdgeLocation, 12> EDGE_LOCATION_ORDER{
      {{Face::U, Face::B},
       {Face::U, Face::R},
       {Face::U, Face::F},
       {Face::U, Face::L},
       {Face::F, Face::R},
       {Face::R, Face::B},
       {Face::B, Face::L},
       {Face::L, Face::F},
       {Face::D, Face::F},
       {Face::D, Face::R},
       {Face::D, Face::B},
       {Face::D, Face::L}}};
  static constexpr std::array<CornerLocation, 8> CORNER_LOCATION_ORDER{
      {{Face::U, Face::L, Face::B},
       {Face::U, Face::B, Face::R},
       {Face::U, Face::R, Face::F},
       {Face::U, Face::F, Face::L},
       {Face::D, Face::L, Face::F},
       {Face::D, Face::F, Face::R},
       {Face::D, Face::R, Face::B},
       {Face::D, Face::B, Face::L}}};
  static constexpr std::array<EdgePiece, 12> STARTING_EDGE_PIECES{
      {{Colour::White, Colour::Blue},
       {Colour::White, Colour::Red},
       {Colour::White, Colour::Green},
       {Colour::White, Colour::Orange},
       {Colour::Green, Colour::Red},
       {Colour::Red, Colour::Blue},
       {Colour::Blue, Colour::Orange},
       {Colour::Orange, Colour::Green},
       {Colour::Yellow, Colour::Green},
       {Colour::Yellow, Colour::Red},
       {Colour::Yellow, Colour::Blue},
       {Colour::Yellow, Colour::Orange}}};
  static constexpr std::array<CornerPiece, 8> STARTING_CORNER_PIECES{
      {{Colour::White, Colour::Orange, Colour::Blue},
       {Colour::White, Colour::Blue, Colour::Red},
       {Colour::White, Colour::Red, Colour::Green},
       {Colour::White, Colour::Green, Colour::Orange},
       {Colour::Yellow, Colour::Orange, Colour::Green},
       {Colour::Yellow, Colour::Green, Colour::Red},
       {Colour::Yellow, Colour::Red, Colour::Blue},
       {Colour::Yellow, Colour::Blue, Colour::Orange}}};
  static constexpr CubeOrientation STARTING_ORIENTATION =
      CubeOrientation::identity();

 private:
  std::array<EdgePiece, 12> edges;
  std::array<CornerPiece, 8> corners;
  CubeOrientation orientation;

 public:
  Cube()
      : edges(STARTING_EDGE_PIECES),
        corners(STARTING_CORNER_PIECES),
        orientation(STARTING_ORIENTATION) {}

  explicit Cube(const Algorithm& scramble) : Cube() { apply(scramble); }

  void apply(const Turn& turn);

  void apply(const SliceTurn& slice_turn);

  void apply(const WideTurn& wide_turn);

  void apply(const CubeRotation& cubeRotation);

  void apply(const Move& move);

  void apply(const Algorithm& algorithm);

  void scramble();

  void setSolved();

  [[nodiscard]] bool edgesSolved() const;

  [[nodiscard]] bool cornersSolved() const;

  [[nodiscard]] bool isSolved() const;

  [[nodiscard]] bool isStandardOrientation() const;

  /**
   * @brief Compares the edges, corners, and orientation for equality
   */
  [[nodiscard]] bool operator==(const Cube& other) const;

  [[nodiscard]] std::string toNetString() const;

  EdgePieceProxy operator[](const EdgeLocation& edge_location);

  ConstEdgePieceProxy operator[](const EdgeLocation& edge_location) const;

  CornerPieceProxy operator[](const CornerLocation& corner_location);

  ConstCornerPieceProxy operator[](const CornerLocation& corner_location) const;

  template <size_t n>
  void cycleEdges(const std::array<EdgeLocation, n>& edge_locations) {
    static_assert(n >= 2, "Must cycle at least 2 edges!");
    EdgePiece next_edge_piece = (*this)[edge_locations.front()];
    for (size_t i = 1; i < edge_locations.size(); i++) {
      const EdgePiece temp = (*this)[edge_locations[i]];
      (*this)[edge_locations[i]] = next_edge_piece;
      next_edge_piece = temp;
    }
    (*this)[edge_locations.front()] = next_edge_piece;
  }

  template <size_t n>
  void cycleCorners(const std::array<CornerLocation, n>& corner_locations) {
    static_assert(n >= 2, "Must cycle at least 2 corners!");
    CornerPiece next_corner_piece = (*this)[corner_locations.front()];
    for (size_t i = 1; i < corner_locations.size(); i++) {
      const CornerPiece temp = (*this)[corner_locations[i]];
      (*this)[corner_locations[i]] = next_corner_piece;
      next_corner_piece = temp;
    }
    (*this)[corner_locations.front()] = next_corner_piece;
  }
};

namespace std {
template <>
struct hash<Cube> {
  size_t operator()(const Cube& cube) const {
    static constexpr std::hash<CubeOrientation> orientation_hasher;
    static constexpr std::hash<EdgePiece> edge_hasher{};
    static constexpr std::hash<CornerPiece> corner_hasher{};
    size_t hash = orientation_hasher(cube.orientation);
    for (EdgePiece edge_piece : cube.edges)
      hash = 31 * hash + edge_hasher(edge_piece);
    for (CornerPiece corner_piece : cube.corners)
      hash = 31 * hash + corner_hasher(corner_piece);
    return hash;
  }
};
}  // namespace std
