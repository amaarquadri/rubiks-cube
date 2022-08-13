#include "HalfTurnReductionSolver.h"
#include "Algorithm.h"
#include "Combination.h"
#include "Cube.h"
#include "DominoReductionSolver.h"
#include "MathUtils.h"
#include "Permutation.h"
#include "SolverUtils.h"
#include "Turn.h"
#include <cassert>

namespace solvers {
struct OptimalMove {
  Turn turn;
  uint16_t next_descriptor;
};

/**
 * All possible Turns that maintain domino reduction.
 */
static constexpr std::array<Turn, 10> PossibleTurns = []() {
  std::array<Turn, 10> possible_turns;
  uint8_t i = 0;
  for (const Face& face : {Face::U, Face::D})
    for (const RotationAmount& rotation_amount :
         {RotationAmount::Clockwise, RotationAmount::HalfTurn,
          RotationAmount::Counterclockwise})
      possible_turns[i++] = Turn{face, rotation_amount};
  for (const Face& face : {Face::F, Face::B, Face::R, Face::L})
    possible_turns[i++] = Turn{face, RotationAmount::HalfTurn};
  assert(i == possible_turns.size());
  return possible_turns;
}();

static constexpr uint16_t CornerCombinationCount = utility::nChooseK(8, 4);
static constexpr uint16_t EdgeCombinationCount = utility::nChooseK(8, 4);
static constexpr uint16_t PiecesCombinationCount =
    EdgeCombinationCount * CornerCombinationCount;
static constexpr uint16_t ParityCount = 2;
// TODO: factor of 3
static constexpr uint16_t DescriptorCount =
    ParityCount * PiecesCombinationCount;
static constexpr uint16_t SolvedDescriptor =
    (CornerCombinationCount + 1) * Combination<8, 4>({0, 2, 4, 6}).getRank();

static constexpr uint16_t applyTurn(const uint16_t& descriptor,
                                    const Turn& turn) {
  assert(turn.rotation_amount != RotationAmount::None);

  std::array<uint8_t, 4> primary_tetrad_corner_locations =
      Combination<8, 4>::parseRank(descriptor % CornerCombinationCount);
  std::array<uint8_t, 4> m_slice_edge_locations = Combination<8, 4>::parseRank(
      (descriptor / CornerCombinationCount) % EdgeCombinationCount);
  bool odd_parity = descriptor / PiecesCombinationCount;

  cycleIndices(primary_tetrad_corner_locations, getCornerCycle(turn.face),
               static_cast<size_t>(turn.rotation_amount));

  // relabel 4, 5, 6, and 7 to 8, 9, 10, and 11 respectively
  std::array<uint8_t, 4> edge_cycle = getEdgeCycle(turn.face);
  for (uint8_t& location : edge_cycle) {
    if (location >= 8)
      location -= 4;
    else if (location >= 4)
      location += 4;
  }
  cycleIndices(m_slice_edge_locations, edge_cycle,
               static_cast<size_t>(turn.rotation_amount));

  // half turns flip the edge and corner parity
  if (turn.rotation_amount != RotationAmount::None) odd_parity = !odd_parity;

  uint16_t new_descriptor =
      primary_tetrad_corner_combination.getRank() +
      m_slice_edge_combination.getRank() * CornerCombinationCount;
  if (odd_parity) new_descriptor += PiecesCombinationCount;
  return new_descriptor;
}

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
  return after_white_yellow == Colour::Red ||
         after_white_yellow == Colour::Orange;
}

static uint16_t getDescriptor(const Cube& cube) {
  static constexpr std::array<uint8_t, 8> Locations{0, 1, 2, 3, 8, 9, 10, 11};

  Combination<8, 4> primary_tetrad_corner_combination;
  uint8_t i = 0;
  for (uint8_t j = 0; j < Cube::CORNER_LOCATION_ORDER.size(); ++j)
    if (isPrimaryTetradCorner(cube[Cube::CORNER_LOCATION_ORDER[j]]))
      primary_tetrad_corner_combination[i++] = j;
  // ensure number of primary tetrad corners is 4
  assert(i == primary_tetrad_corner_combination.size());

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
      getSolver<DescriptorCount, PossibleTurns, applyTurn, SolvedDescriptor>();

  const Algorithm domino_reduction_solve = solveDominoReduction(cube);
  cube.apply(domino_reduction_solve);
  return domino_reduction_solve + solver(getDescriptor(cube));
}
}  // namespace solvers
