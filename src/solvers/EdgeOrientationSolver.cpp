#include "EdgeOrientationSolver.h"
#include "Algorithm.h"
#include "Cube.h"
#include "Move.h"
#include "SolverUtils.h"
#include "StaticVector.h"
#include "Turn.h"
#include "TurnSets.h"
#include <array>
#include <bit>
#include <cstdint>
#include <stdexcept>
#include <utility>

namespace solvers {
static constexpr uint16_t DescriptorCount = 2048;
static constexpr uint16_t SolvedDescriptor = 0;

template <size_t n>
static constexpr void cycleEdges(uint16_t& edge_orientation,
                                 const std::array<uint8_t, n>& edge_cycle) {
  if constexpr (n < 2)
    throw std::invalid_argument("Must cycle at least 2 edges!");

  bool next_bit = edge_orientation & (1 << edge_cycle.front());
  const auto set_next_bit = [&](const uint8_t& index) {
    if (next_bit)
      edge_orientation |= (1 << index);
    else
      edge_orientation &= ~(1 << index);
  };

  for (size_t i = 1; i < edge_cycle.size(); i++) {
    const bool temp = edge_orientation & (1 << edge_cycle[i]);
    set_next_bit(edge_cycle[i]);
    next_bit = temp;
  }
  set_next_bit(edge_cycle.front());
}

static constexpr uint16_t applyTurn(uint16_t edge_orientation,
                                    const Turn& turn) {
  assert(turn.rotation_amount != RotationAmount::None);

  // temporarily add explicit 12th bit
  if (std::popcount(edge_orientation) % 2 == 1) edge_orientation |= (1 << 11);

  const std::array<uint8_t, 4> edge_cycle = getEdgeCycle(turn.face);

  switch (turn.rotation_amount) {
    case RotationAmount::Clockwise:
      cycleEdges<4>(edge_orientation, edge_cycle);
      break;
    case RotationAmount::HalfTurn:
      cycleEdges<2>(edge_orientation, {edge_cycle[0], edge_cycle[2]});
      cycleEdges<2>(edge_orientation, {edge_cycle[1], edge_cycle[3]});
      break;
    case RotationAmount::Counterclockwise:
      cycleEdges<4>(edge_orientation, {edge_cycle[3], edge_cycle[2],
                                       edge_cycle[1], edge_cycle[0]});
      break;
    default:
      // RotationAmount::None was already checked for at the beginning
      throw std::logic_error("Unknown enum value!");
  }

  // for F, F', B, and B' flip the correct bits since the edge orientation flips
  if ((turn.face == Face::F || turn.face == Face::B) &&
      turn.rotation_amount != RotationAmount::HalfTurn)
    for (const size_t& idx : edge_cycle) edge_orientation ^= (1 << idx);

  // remove temporary redundant 12th bit
  edge_orientation &= ~(1 << 11);

  return edge_orientation;
}

/**
 * Computes an uint16_t whose lower 11 bits represent whether or not the
 * corresponding edges of the given cube are flipped or not. The edge piece at
 * Cube::EDGE_LOCATION_ORDER[i] corresponds to the (0-indexed) i-th lowest bit
 * of the result. The bit will be 1 if and only if the corresponding edge is
 * incorrectly oriented. The 12th bit can be inferred using the following
 * expression: std::popcount(edge_orientation) % 2 == 1.
 */
static uint16_t getEdgeOrientation(const Cube& cube) {
  uint16_t edge_orientation = 0;
  for (size_t i = 0; i < Cube::EDGE_LOCATION_ORDER.size() - 1; ++i) {
    const EdgeLocation& target = (i == 5 || i == 7)
                                     ? Cube::EDGE_LOCATION_ORDER[i].flip()
                                     : Cube::EDGE_LOCATION_ORDER[i];
    const EdgePiece edge = cube[target];
    const bool is_flipped =
        edge.second == Colour::White || edge.second == Colour::Yellow ||
        edge.first == Colour::Red || edge.first == Colour::Orange;
    if (is_flipped) edge_orientation |= (1 << i);
  }
  return edge_orientation;
}

bool areEdgesOriented(const Cube& cube) {
  return getEdgeOrientation(cube) == SolvedDescriptor;
}

Algorithm solveEdgeOrientation(const Cube& cube) {
  static constexpr auto solver = getSolver<DescriptorCount, AllPossibleTurns,
                                           applyTurn, SolvedDescriptor>();
  return solver(getEdgeOrientation(cube));
}

// Test functions

static void testGetEdgeOrientation() {
  if (getEdgeOrientation(Cube{}) != SolvedDescriptor ||
      getEdgeOrientation(Cube{Algorithm::parse("F")}) != 404 ||
      getEdgeOrientation(Cube{Algorithm::parse("F R")}) != 390 ||
      getEdgeOrientation(Cube{Algorithm::parse("F B L U D' B2 R2 F")}) !=
          (DescriptorCount - 1))
    throw std::logic_error("Incorrect edge orientation!");
}

static void testApplyTurn() {
  static constexpr size_t count = 1000;

  for (size_t i = 0; i < count; ++i) {
    const Algorithm alg = Algorithm::random(20);
    uint16_t edge_orientation = SolvedDescriptor;
    for (const Move& move : alg)
      edge_orientation = applyTurn(edge_orientation, move.getTurn());
    if (edge_orientation != getEdgeOrientation(Cube{alg}))
      throw std::logic_error("Edge orientation mismatch!");
  }
}

void runEdgeOrientationSolverTests() {
  testGetEdgeOrientation();
  testApplyTurn();
}
}  // namespace solvers
