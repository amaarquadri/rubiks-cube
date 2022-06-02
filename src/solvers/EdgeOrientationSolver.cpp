#include "EdgeOrientationSolver.h"
#include "Algorithm.h"
#include "Cube.h"
#include "StaticVector.h"
#include <array>
#include <bit>
#include <cstdint>
#include <stdexcept>

namespace solvers {
struct OptimalMove {
  Turn turn;
  unsigned short next_edge_orientation;
};

static constexpr std::array<Turn, 18> getPossibleTurns() {
  std::array<Turn, 18> possible_turns{};
  uint8_t i = 0;
  for (const Face& face :
       {Face::U, Face::F, Face::R, Face::B, Face::L, Face::D})
    for (const RotationAmount& rotation_amount :
         {RotationAmount::Clockwise, RotationAmount::HalfTurn,
          RotationAmount::Counterclockwise})
      possible_turns[i++] = Turn{face, rotation_amount};
  return possible_turns;
}

static constexpr std::array<Turn, 18> PossibleTurns = getPossibleTurns();

template <size_t n>
static constexpr void cycleEdges(unsigned short& edge_orientation,
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

static constexpr unsigned short applyTurn(unsigned short edge_orientation,
                                          const Turn& turn) {
  if (turn.is_wide_turn || turn.is_slice_turn)
    throw std::invalid_argument("Turn cannot be a wide turn or a slice turn!");
  if (turn.rotation_amount == RotationAmount::None) return edge_orientation;

  // temporarily add explicit 12th bit
  if (std::popcount(edge_orientation) % 2 == 1) edge_orientation |= (1 << 11);

  const std::array<uint8_t, 4> edge_cycle = [&]() {
    switch (turn.face) {
      case Face::U:
        return std::array<uint8_t, 4>{0, 1, 2, 3};
      case Face::F:
        return std::array<uint8_t, 4>{2, 4, 8, 7};
      case Face::R:
        return std::array<uint8_t, 4>{1, 5, 9, 4};
      case Face::B:
        return std::array<uint8_t, 4>{0, 6, 10, 5};
      case Face::L:
        return std::array<uint8_t, 4>{3, 7, 11, 6};
      case Face::D:
        return std::array<uint8_t, 4>{8, 9, 10, 11};
      default:
        throw std::logic_error("Unknown enum value!");
    }
  }();

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

static constexpr std::array<OptimalMove, 2048> getOptimalMoves() {
  constexpr unsigned short UnknownSentinel = 2048;

  std::array<OptimalMove, 2048> optimal_moves{};
  for (size_t i = 0; i < 2048; ++i)
    optimal_moves[i].next_edge_orientation = UnknownSentinel;

  utility::StaticVector<unsigned short, 2048> current{};
  current.push_back(0);
  utility::StaticVector<unsigned short, 2048> next{};

  while (current.getSize() > 0) {
    for (const unsigned short& idx : current) {
      for (const Turn& turn : PossibleTurns) {
        const unsigned short next_idx = applyTurn(idx, turn);
        if (next_idx != 0 &&
            optimal_moves[next_idx].next_edge_orientation == UnknownSentinel) {
          optimal_moves[next_idx].turn = turn.inv();
          optimal_moves[next_idx].next_edge_orientation = idx;
          next.push_back(next_idx);
        }
      }
    }
    current = next;
    next.clear();
  }
  return optimal_moves;
}

static constexpr std::array<OptimalMove, 2048> OptimalMoves = getOptimalMoves();

/**
 * Computes an unsigned short whose lower 11 bits represent whether or not the
 * corresponding edges of the given cube are flipped or not. The edge piece at
 * Cube::EDGE_LOCATION_ORDER[i] corresponds to the (0-indexed) i-th lowest bit
 * of the result. The bit will be 1 if and only if the corresponding edge is
 * incorrectly oriented. The 12th bit can be inferred using the following
 * expression: std::popcount(edge_orientation) % 2 == 1.
 */
static unsigned short getEdgeOrientation(const Cube& cube) {
  unsigned short edge_orientation = 0;
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
  return getEdgeOrientation(cube) == 0;
}

Algorithm solveEdgeOrientation(const Cube& cube) {
  Algorithm alg;
  unsigned short edge_orientation = getEdgeOrientation(cube);
  while (edge_orientation != 0) {
    const OptimalMove& optimal_move = OptimalMoves[edge_orientation];
    alg.push_back(Move{optimal_move.turn});
    edge_orientation = optimal_move.next_edge_orientation;
  }
  return alg;
}

// Test functions

void testGetEdgeOrientation() {
  if (getEdgeOrientation(Cube{}) != 0 ||
      getEdgeOrientation(Cube{Algorithm::parse("F")}) != 404 ||
      getEdgeOrientation(Cube{Algorithm::parse("F R")}) != 390 ||
      getEdgeOrientation(Cube{Algorithm::parse("F B L U D' B2 R2 F")}) != 2047)
    throw std::logic_error("Incorrect edge orientation!");
}

void testApplyTurn() {
  static constexpr size_t count = 1000;

  for (size_t i = 0; i < count; ++i) {
    const Algorithm alg = Algorithm::random(20);
    unsigned short edge_orientation = 0;
    for (const Move& move : alg)
      edge_orientation = applyTurn(edge_orientation, move.turn);
    if (edge_orientation != getEdgeOrientation(Cube{alg}))
      throw std::logic_error("Edge orientation mismatch!");
  }
}

void testSolveEdgeOrientation() {
  static constexpr size_t count = 1000;

  for (size_t i = 0; i < count; ++i) {
    const Algorithm scramble = Algorithm::random(20);
    Cube cube{scramble};
    const Algorithm solve = solveEdgeOrientation(cube);
    cube.apply(solve);
    if (getEdgeOrientation(cube) != 0)
      throw std::logic_error("Edge orientation was not solved!");
  }
}
}  // namespace solvers
