#include "HalfTurnReductionSolver.h"
#include "Algorithm.h"
#include "Combination.h"
#include "Cube.h"
#include "DominoReductionSolver.h"
#include "MathUtils.h"
#include "Permutation.h"
#include "SolverUtils.h"
#include "Turn.h"
#include "TurnSets.h"
#include <array>
#include <cassert>

namespace solvers {
static constexpr uint16_t CornerCombinationCount = utility::nChooseK(8, 4);
static constexpr uint16_t EdgeCombinationCount = utility::nChooseK(8, 4);
static constexpr uint16_t PiecesCombinationCount =
    EdgeCombinationCount * CornerCombinationCount;
static constexpr uint16_t ParityCount = 2;
static constexpr uint16_t ParityAndPiecesCombinationCount =
    ParityCount * PiecesCombinationCount;
static constexpr uint16_t CornerThreeParityCount = 3;
static constexpr uint16_t DescriptorCount =
    CornerThreeParityCount * ParityAndPiecesCombinationCount;
static constexpr uint16_t SolvedDescriptor =
    (CornerCombinationCount + 1) * Combination<8, 4>({0, 2, 4, 6}).getRank();

static constexpr uint16_t applyTurn(const uint16_t& descriptor,
                                    const Turn& turn) {
  assert(turn.rotation_amount != RotationAmount::None);

  Combination<8, 4> primary_tetrad_corner_locations =
      Combination<8, 4>::parseRank(descriptor % CornerCombinationCount);
  Combination<8, 4> m_slice_edge_locations = Combination<8, 4>::parseRank(
      (descriptor / CornerCombinationCount) % EdgeCombinationCount);
  bool odd_parity = descriptor / PiecesCombinationCount;

  cycleValues(primary_tetrad_corner_locations, getCornerCycle(turn.face),
              static_cast<size_t>(turn.rotation_amount));

  // relabel 4, 5, 6, and 7 to 8, 9, 10, and 11 respectively
  std::array<uint8_t, 4> edge_cycle = getEdgeCycle(turn.face);
  for (uint8_t& location : edge_cycle) {
    if (location >= 8)
      location -= 4;
    else if (location >= 4)
      location += 4;
  }
  cycleValues(m_slice_edge_locations, edge_cycle,
              static_cast<size_t>(turn.rotation_amount));

  // half turns flip the edge and corner parity
  if (turn.rotation_amount != RotationAmount::None) odd_parity = !odd_parity;

  uint16_t new_descriptor =
      primary_tetrad_corner_combination.getRank() +
      m_slice_edge_combination.getRank() * CornerCombinationCount;
  if (odd_parity) new_descriptor += PiecesCombinationCount;
  return new_descriptor;
}

/**
 * Returns true if and only if this CornerPiece would is positioned in an even
 * index in the solved state.
 */
static constexpr bool isPrimaryTetradCorner(const CornerPiece& corner) {
  const std::array<Colour, 3> colours{corner.first, corner.second,
                                      corner.third};
  const uint8_t white_yellow_index =
      std::find_if(colours.begin(), colours.end(),
                   [](const Colour& colour) {
                     return colour == Colour::White || colour == Colour::Yellow;
                   }) -
      colours.begin();
  assert(white_yellow_index != 3);  // ensure white or yellow was found
  const Colour& after_white_yellow = colours[(white_yellow_index + 1) % 3];
  // return true if the colour after (clockwise of) white/yellow is red/orange
  // return false if the colour after (clockwise of) white/yellow is green/blue
  return after_white_yellow == Colour::Red ||
         after_white_yellow == Colour::Orange;
}

static uint16_t getDescriptor(const Cube& cube) {
  Combination<8, 4> primary_tetrad_corner_combination;
  uint8_t i = 0;
  for (uint8_t j = 0; j < Cube::CORNER_LOCATION_ORDER.size(); ++j)
    if (isPrimaryTetradCorner(cube[Cube::CORNER_LOCATION_ORDER[j]]))
      primary_tetrad_corner_combination[i++] = j;
  // ensure number of primary tetrad corners is 4
  assert(i == primary_tetrad_corner_combination.size());

  static constexpr std::array<uint8_t, 8> Locations{0, 1, 2, 3, 8, 9, 10, 11};
  Combination<8, 4> m_slice_edge_combination;
  i = 0;
  for (uint8_t j = 0; j < Locations.size(); ++j) {
    const EdgePiece edge = cube[Cube::EDGE_LOCATION_ORDER[Locations[j]]];
    if (edge.first != Colour::Red && edge.first != Colour::Orange &&
        edge.second != Colour::Red && edge.second != Colour::Orange)
      m_slice_edge_combination[i++] = j;
  }
  // ensure number of M slice edges is 4
  assert(i == m_slice_edge_combination.size());

  Permutation<8> corner_permutation;
  for (uint8_t j = 0; j < 8; ++j) {
    const CornerPiece corner = cube[Cube::CORNER_LOCATION_ORDER[j]];
    corner_permutation[j] =
        std::find(Cube::STARTING_CORNER_PIECES.begin(),
                  Cube::STARTING_CORNER_PIECES.end(), corner) -
        Cube::STARTING_CORNER_PIECES.begin();
    assert(corner_permutation[j] != Cube::STARTING_CORNER_PIECES.size());
  }

  // TODO: compute factor of 3
  uint16_t descriptor =
      primary_tetrad_corner_combination.getRank() +
      m_slice_edge_combination.getRank() * CornerCombinationCount;
  if (corner_permutation.isOdd()) descriptor += PiecesCombinationCount;
  return descriptor;
}

bool isHalfTurnReduced(const Cube& cube) {
  return isDominoReduced(cube) && getDescriptor(cube) == SolvedDescriptor;
}

Algorithm solveHalfTurnReduction(Cube cube) {
  static constexpr auto solver =
      getSolver<DescriptorCount, DominoReductionPreservingTurns, applyTurn,
                SolvedDescriptor>();

  const Algorithm domino_reduction_solve = solveDominoReduction(cube);
  cube.apply(domino_reduction_solve);
  return domino_reduction_solve + solver(getDescriptor(cube));
}
}  // namespace solvers
