#pragma once

#include "Algorithm.h"
#include "CornerLocation.h"
#include "CornerPiece.h"
#include "CubeOrientation.h"
#include "CubeRotation.h"
#include "EdgeLocation.h"
#include "EdgePiece.h"
#include "Move.h"
#include "SolveData.h"
#include "Turn.h"
#include <array>
#include <string>
#include <vector>

class Cube;

namespace blindsolving {
using Reconstruction = std::vector<SolveData>;
std::vector<EdgeLocation> getUnsolvedEdges(const Cube& cube);
std::vector<CornerLocation> getUnsolvedCorners(const Cube& cube);
std::vector<Reconstruction> getPossibleReconstructions(Cube& cube);
}  // namespace blindsolving

class Cube {
 public:
  friend std::vector<blindsolving::Reconstruction>
  blindsolving::getPossibleReconstructions(Cube& cube);
  friend std::vector<EdgeLocation> blindsolving::getUnsolvedEdges(
      const Cube& cube);
  friend std::vector<CornerLocation> blindsolving::getUnsolvedCorners(
      const Cube& cube);
  friend struct std::hash<Cube>;

  constexpr static const std::array<EdgeLocation, 12> EDGE_LOCATION_ORDER{
      {{U, B},
       {U, R},
       {U, F},
       {U, L},
       {F, R},
       {R, B},
       {B, L},
       {L, F},
       {D, F},
       {D, R},
       {D, B},
       {D, L}}};
  constexpr static const std::array<CornerLocation, 8> CORNER_LOCATION_ORDER{
      {{U, L, B},
       {U, B, R},
       {U, R, F},
       {U, F, L},
       {D, L, F},
       {D, F, R},
       {D, R, B},
       {D, B, L}}};
  constexpr static const std::array<EdgePiece, 12> STARTING_EDGE_PIECES{
      {{WHITE, BLUE},
       {WHITE, RED},
       {WHITE, GREEN},
       {WHITE, ORANGE},
       {GREEN, RED},
       {RED, BLUE},
       {BLUE, ORANGE},
       {ORANGE, GREEN},
       {YELLOW, GREEN},
       {YELLOW, RED},
       {YELLOW, BLUE},
       {YELLOW, ORANGE}}};
  constexpr static const std::array<CornerPiece, 8> STARTING_CORNER_PIECES{
      {{WHITE, ORANGE, BLUE},
       {WHITE, BLUE, RED},
       {WHITE, RED, GREEN},
       {WHITE, GREEN, ORANGE},
       {YELLOW, ORANGE, GREEN},
       {YELLOW, GREEN, RED},
       {YELLOW, RED, BLUE},
       {YELLOW, BLUE, ORANGE}}};
  constexpr static const CubeOrientation STARTING_ORIENTATION{U, F};

 private:
  std::array<EdgePiece, 12> edges;
  std::array<CornerPiece, 8> corners;
  CubeOrientation orientation;

 public:
  Cube()
      : edges(STARTING_EDGE_PIECES),
        corners(STARTING_CORNER_PIECES),
        orientation(STARTING_ORIENTATION) {}

  void apply(const Turn& turn);

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

 private:
  [[nodiscard]] EdgePiece getEdge(const EdgeLocation& edgeLocation) const;

  void setEdge(const EdgePiece& edgePiece, const EdgeLocation& edgeLocation);

  [[nodiscard]] CornerPiece getCorner(
      const CornerLocation& cornerLocation) const;

  void setCorner(const CornerPiece& cornerPiece,
                 const CornerLocation& cornerLocation);

  void cycleEdges(const std::vector<EdgeLocation>& edgeLocations);

  void cycleCorners(const std::vector<CornerLocation>& cornerLocations);
};

namespace std {
template <>
struct hash<Cube> {
  size_t operator()(const Cube& cube) const {
    size_t hash = std::hash<CubeOrientation>{}(cube.orientation);
    for (EdgePiece edge_piece : cube.edges) {
      hash = 31 * hash + std::hash<EdgePiece>{}(edge_piece);
    }
    for (CornerPiece corner_piece : cube.corners) {
      hash = 31 * hash + std::hash<CornerPiece>{}(corner_piece);
    }
    return hash;
  }
};
}  // namespace std
