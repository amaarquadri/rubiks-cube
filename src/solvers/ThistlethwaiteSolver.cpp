#include "ThistlethwaiteSolver.h"
#include "Algorithm.h"
#include "Cube.h"
#include "Face.h"
#include "HalfTurnReductionSolver.h"
#include "MathUtils.h"
#include "Permutation.h"
#include "RotationAmount.h"
#include "SolverUtils.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <stdexcept>

namespace solvers {
/**
 * All possible Turns that maintain half turn reduction.
 */
static constexpr std::array<Turn, 6> PossibleTurns = []() {
  using enum Face;
  std::array<Turn, 6> possible_turns;
  uint8_t i = 0;
  for (const Face& face : {U, F, R, B, L, D})
    possible_turns[i++] = Turn{face, RotationAmount::HalfTurn};
  assert(i == possible_turns.size());
  return possible_turns;
}();

static constexpr uint32_t MSlicePermutationCount = utility::factorial(4);
static constexpr uint32_t SSlicePermutationCount = utility::factorial(4);
static constexpr uint32_t MSSlicePermutationCounts =
    MSlicePermutationCount * SSlicePermutationCount;
static constexpr uint32_t ESlicePermutationCount = utility::factorial(4) / 2;
static constexpr uint32_t EdgePermutationCount =
    MSSlicePermutationCounts * ESlicePermutationCount;
static constexpr uint32_t PrimaryTetradPermutationCount = utility::factorial(4);
static constexpr uint32_t PrimaryTetradAndEdgesPermutationCount =
    EdgePermutationCount * PrimaryTetradPermutationCount;
static constexpr uint32_t SecondaryTetradPermutationCount =
    utility::factorial(4) / 6;
static constexpr uint32_t DescriptorCount =
    PrimaryTetradAndEdgesPermutationCount * SecondaryTetradPermutationCount;

static constexpr uint32_t applyTurn(const uint32_t& descriptor,
                                    const Turn& turn) {
  assert(turn.rotation_amount == RotationAmount::HalfTurn);

  Permutation<4> m_slice_permutation =
      Permutation<4>::parseRank(descriptor % MSlicePermutationCount);
  Permutation<4> s_slice_permutation = Permutation<4>::parseRank(
      (descriptor / MSlicePermutationCount) % SSlicePermutationCount);
  Permutation<4> e_slice_permutation = Permutation<4>::parseParityRank(
      (descriptor / MSSlicePermutationCounts) % ESlicePermutationCount,
      m_slice_permutation.isOdd() != s_slice_permutation.isOdd());
  Permutation<4> primary_tetrad_permutation = Permutation<4>::parseRank(
      (descriptor / EdgePermutationCount) % PrimaryTetradPermutationCount);
  // TODO: factor of 3
  Permutation<4> secondary_tetrad_permutation = Permutation<4>::parseParityRank(
      descriptor / PrimaryTetradAndEdgesPermutationCount,
      primary_tetrad_permutation.isOdd());



  return m_slice_permutation.getRank() +
         MSlicePermutationCount * s_slice_permutation.getRank() +
         MSSlicePermutationCounts * e_slice_permutation.getParityRank() +
         EdgePermutationCount * primary_tetrad_permutation.getRank() +
         PrimaryTetradAndEdgesPermutationCount *
             secondary_tetrad_permutation.getParityRank() / 3;
}

static uint32_t getDescriptor(const Cube& cube) {
  static constexpr std::array<uint8_t, 12> Mapping{0, 0, 1, 1, 0, 1,
                                                   2, 3, 2, 3, 3, 2};
  const auto get_edge_location_index = [](const EdgePiece& edge_piece) {
    for (size_t i = 0; i < Cube::EDGE_LOCATION_ORDER.size(); ++i) {
      const EdgePiece target = (i == 5 || i == 7)
                                   ? Cube::STARTING_EDGE_PIECES[i].flip()
                                   : Cube::STARTING_EDGE_PIECES[i];
      if (edge_piece == target) return i;
    }
    throw std::invalid_argument("EdgePiece was not oriented!")
  };
  const auto get_corner_location_index = [](const CornerPiece& corner_piece) {
    for (size_t i = 0; i < Cube::CORNER_LOCATION_ORDER.size(); ++i)
      if (corner_piece == Cube::STARTING_CORNER_PIECES[i]) return i;
    throw std::invalid_argument("CornerPiece was not oriented!")
  };

  Permutation<4> m_slice_permutation;
  uint8_t i = 0;
  for (size_t location_index : {0, 2, 8, 10})
    m_slice_permutation[i++] = Mapping[get_edge_location_index(
        cube[Cube::EDGE_LOCATION_ORDER[location_index]])];
  assert(i == m_slice_permutation.size());

  Permutation<4> s_slice_permutation;
  i = 0;
  for (size_t location_index : {1, 3, 11, 9})
    s_slice_permutation[i++] = Mapping[get_edge_location_index(
        cube[Cube::EDGE_LOCATION_ORDER[location_index]])];
  assert(i == s_slice_permutation.size());

  Permutation<4> e_slice_permutation;
  i = 0;
  for (size_t location_index = 4; location_index < 8; ++location_index)
    e_slice_permutation[i++] = Mapping[get_edge_location_index(
        cube[Cube::EDGE_LOCATION_ORDER[location_index]])];
  assert(i == e_slice_permutation.size());

  Permutation<4> primary_tetrad_permutation;
  i = 0;
  for (size_t location_index = 0; location_index < 8; location_index += 2)
    primary_tetrad_permutation[i++] =
        get_corner_location_index(
            cube[Cube::CORNER_LOCATION_ORDER[location_index]]) /
        2;
  assert(i == primary_tetrad_permutation.size());

  Permutation<4> secondary_tetrad_permutation;
  i = 0;
  for (size_t location_index = 1; location_index < 8; location_index += 2)
    secondary_tetrad_permutation[i++] =
        get_corner_location_index(
            cube[Cube::CORNER_LOCATION_ORDER[location_index]]) /
        2;
  assert(i == secondary_tetrad_permutation.size());

  return m_slice_permutation.getRank() +
         MSlicePermutationCount * s_slice_permutation.getRank() +
         MSSlicePermutationCounts * e_slice_permutation.getParityRank() +
         EdgePermutationCount * primary_tetrad_permutation.getRank() +
         PrimaryTetradAndEdgesPermutationCount *
             secondary_tetrad_permutation.getParityRank() / 3;
}

Algorithm solveThistlethwaite(Cube cube) {
  static constexpr auto solver =
      getSolver<DescriptorCount, PossibleTurns, applyTurn, SolvedDescriptor>();

  const Algorithm half_turn_reduction_solve = solveHalfTurnReduction(cube);
  cube.apply(half_turn_reduction_solve);
  return half_turn_reduction_solve + solver(getDescriptor(cube));
}

void testIsSolved() {
  // Should be identical to cube.isSolved()
  const auto is_solved = [](const Cube& cube) {
    return isHalfTurnReduced(cube) && getDescriptor(cube) == SolvedDescriptor;
  };

  const Cube solved_cube{};
  assert(solved_cube.isSolved());
  if (!is_solved(solved_cube)) throw std::logic_error("isSolved disagreement!");

  static constexpr size_t Count = 1000;

  for (size_t i = 0; i < Count; ++i) {
    const Cube cube = Cube{Algorithm::random(20)};
    if (is_solved(cube) != cube.isSolved())
      throw std::logic_error("isSolved disagreement!");
  }
}

void runThistlethwaiteSolverTests() { testIsSolved(); }
}  // namespace solvers
