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
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

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
    Combination<8, 4>({0, 2, 4, 6}).getRank();

static constexpr uint16_t getCornerThreeParity(
    const Permutation<8>& corner_permutation,
    const Combination<8, 4>& primary_tetrad_corner_combination) {
  // extract relative permutation of each tetrad
  Permutation<4> primary_tetrad_permutation;
  Permutation<4> secondary_tetrad_permutation;
  uint8_t i = 0;  // number of assigned elements of primary_tetrad_permutation
  for (uint8_t j = 0; j < 8; ++j) {
    if (i < primary_tetrad_permutation.size() &&
        primary_tetrad_corner_combination[i] == j)
      primary_tetrad_permutation[i++] = j / 2;
    else {
      assert(j >= i);
      secondary_tetrad_permutation[j - i] = j / 2;
    }
  }
  assert(i == primary_tetrad_permutation.size());
  assert(primary_tetrad_permutation.isValid());
  assert(secondary_tetrad_permutation.isValid());

  /** static **/ constexpr std::array<uint8_t, 24> TetradThreeParity{
      0, 2, 1, 1, 2, 0, 2, 0, 2, 0, 1, 1, 1, 1, 0, 2, 0, 2, 0, 2, 1, 1, 2, 0};
  static_assert(std::equal(TetradThreeParity.begin(), TetradThreeParity.end(),
                           TetradThreeParity.rbegin(),
                           TetradThreeParity.rend()));
  return (TetradThreeParity[primary_tetrad_permutation.getRank()] +
          TetradThreeParity[secondary_tetrad_permutation.getRank()]) %
         3;
}

static constexpr uint16_t applyTurn(const uint16_t& descriptor,
                                    const Turn& turn) {
  assert(turn.rotation_amount != RotationAmount::None);

  Combination<8, 4> primary_tetrad_corner_combination =
      Combination<8, 4>::parseRank(descriptor % CornerCombinationCount);
  Combination<8, 4> m_slice_edge_combination = Combination<8, 4>::parseRank(
      (descriptor / CornerCombinationCount) % EdgeCombinationCount);

  cycleValues(primary_tetrad_corner_combination, getCornerCycle(turn.face),
              static_cast<uint8_t>(turn.rotation_amount));
  assert(primary_tetrad_corner_combination.isValid());

  cycleValues(m_slice_edge_combination, getEdgeCycle(turn.face),
              static_cast<uint8_t>(turn.rotation_amount));
  assert(m_slice_edge_combination.isValid());

  // half turns flip the edge and corner parity
  bool odd_parity =
      (descriptor % ParityAndPiecesCombinationCount) / PiecesCombinationCount;
  odd_parity ^= turn.rotation_amount != RotationAmount::HalfTurn;

  return primary_tetrad_corner_combination.getRank() +
         m_slice_edge_combination.getRank() * CornerCombinationCount +
         (odd_parity ? PiecesCombinationCount : 0);
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
  for (uint8_t j = 0; j < Cube::CornerLocationOrder.size(); ++j)
    if (isPrimaryTetradCorner(cube.getCornerByIndex(j)))
      primary_tetrad_corner_combination[i++] = j;
  // ensure number of primary tetrad corners is 4
  assert(i == primary_tetrad_corner_combination.size());
  assert(primary_tetrad_corner_combination.isValid());

  /**
   * Restrict search area for M slice edges to the M and S slices since all the
   * E slice edges are in the correct slice already.
   */
  static constexpr std::array<size_t, 8> MAndSSliceEdgeIndices{0, 3, 6, 9,
                                                               1, 4, 7, 10};
  static constexpr auto is_m_slice_edge = [](const EdgePiece& edge) {
    return edge.first != Colour::Red && edge.first != Colour::Orange &&
           edge.second != Colour::Red && edge.second != Colour::Orange;
  };
  Combination<8, 4> m_slice_edge_combination;
  i = 0;
  for (size_t j = 0; j < MAndSSliceEdgeIndices.size(); ++j)
    if (is_m_slice_edge(cube.getEdgeByIndex(MAndSSliceEdgeIndices[j])))
      m_slice_edge_combination[i++] = j;
  // ensure number of M slice edges is 4
  assert(i == m_slice_edge_combination.size());
  assert(m_slice_edge_combination.isValid());

  Permutation<8> corner_permutation;
  for (uint8_t j = 0; j < 8; ++j) {
    const CornerPiece& corner = cube.getCornerByIndex(j);
    corner_permutation[j] =
        std::find(Cube::StartingCornerPieces.begin(),
                  Cube::StartingCornerPieces.end(), corner) -
        Cube::StartingCornerPieces.begin();
    assert(corner_permutation[j] != Cube::StartingCornerPieces.size());
  }
  assert(corner_permutation.isValid());
  const uint16_t corner_three_parity = getCornerThreeParity(
      corner_permutation, primary_tetrad_corner_combination);

  return primary_tetrad_corner_combination.getRank() +
         m_slice_edge_combination.getRank() * CornerCombinationCount +
         (corner_permutation.isOdd() ? PiecesCombinationCount : 0) +
         corner_three_parity * ParityAndPiecesCombinationCount;
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

void runHalfTurnReductionSolverTests() {}
}  // namespace solvers
