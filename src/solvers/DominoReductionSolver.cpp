#include "DominoReductionSolver.h"
#include "Algorithm.h"
#include "Combination.h"
#include "CornerRotationAmount.h"
#include "Cube.h"
#include "EdgeOrientationSolver.h"
#include "MathUtils.h"
#include "PackedBitsArray.h"
#include "RandomUtils.h"
#include "RotationAmount.h"
#include "SolverUtils.h"
#include "StaticVector.h"
#include "Turn.h"
#include "Utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <vector>

namespace solvers {
/**
 * All possible Turns that maintain edge orientation.
 */
static constexpr std::array<Turn, 14> PossibleTurns = []() {
  std::array<Turn, 14> possible_turns;
  uint8_t i = 0;
  for (const Face& face : {Face::U, Face::R, Face::L, Face::D})
    for (const RotationAmount& rotation_amount :
         {RotationAmount::Clockwise, RotationAmount::HalfTurn,
          RotationAmount::Counterclockwise})
      possible_turns[i++] = Turn{face, rotation_amount};
  for (const Face& face : {Face::F, Face::B})
    possible_turns[i++] = Turn{face, RotationAmount::HalfTurn};
  assert(i == possible_turns.size());
  return possible_turns;
}();

static constexpr uint32_t CornerOrientationCount = utility::pow(3, 7);
static constexpr uint32_t EdgeCombinationCount = utility::nChooseK(12, 4);
static constexpr uint32_t DescriptorCount =
    EdgeCombinationCount * CornerOrientationCount;
static constexpr uint32_t SolvedDescriptor =
    CornerOrientationCount * Combination<12, 4>({4, 5, 6, 7}).getRank();

static constexpr uint32_t applyTurn(const uint32_t& descriptor,
                                    const Turn& turn) {
  assert(turn.rotation_amount != RotationAmount::None);

  // parse e_slice_edge_combination and corner_orientations
  Combination<12, 4> e_slice_edge_combination =
      Combination<12, 4>::parseRank(descriptor / CornerOrientationCount);
  std::array<CornerRotationAmount, 8> corner_orientations;
  for (size_t i = 0; i < corner_orientations.size() - 1; ++i)
    corner_orientations[i] = static_cast<CornerRotationAmount>(
        (descriptor / utility::pow(3, i)) % 3);
  corner_orientations.back() = -std::accumulate(corner_orientations.begin(),
                                                corner_orientations.end() - 1,
                                                CornerRotationAmount::None);

  // update e_slice_edge_combination
  cycleValues(e_slice_edge_combination, getEdgeCycle(turn.face),
              static_cast<size_t>(turn.rotation_amount));

  // update corner_orientations
  const std::array<uint8_t, 4> corner_cycle = getCornerCycle(turn.face);
  if (turn.face != Face::U && turn.face != Face::D &&
      turn.rotation_amount != RotationAmount::HalfTurn) {
    corner_orientations[corner_cycle[0]] += CornerRotationAmount::Clockwise;
    corner_orientations[corner_cycle[1]] +=
        CornerRotationAmount::Counterclockwise;
    corner_orientations[corner_cycle[2]] += CornerRotationAmount::Clockwise;
    corner_orientations[corner_cycle[3]] +=
        CornerRotationAmount::Counterclockwise;
  }
  switch (turn.rotation_amount) {
    case RotationAmount::Clockwise:
      utility::cycleArray(corner_orientations, corner_cycle);
      break;
    case RotationAmount::HalfTurn:
      utility::cycleArray(
          corner_orientations,
          std::array<uint8_t, 2>{corner_cycle[0], corner_cycle[2]});
      utility::cycleArray(
          corner_orientations,
          std::array<uint8_t, 2>{corner_cycle[1], corner_cycle[3]});
      break;
    case RotationAmount::Counterclockwise:
      utility::cycleArray(
          corner_orientations,
          std::array<uint8_t, 4>{corner_cycle[3], corner_cycle[2],
                                 corner_cycle[1], corner_cycle[0]});
      break;
    default:
      // RotationAmount::None was already checked for at the beginning
      throw std::logic_error("Unknown enum value!");
  }

  // re-encode e_slice_edge_combination and corner_orientations into a
  // descriptor
  uint32_t new_descriptor = 0;
  for (size_t i = 0; i < 7; ++i)
    new_descriptor +=
        static_cast<uint32_t>(corner_orientations[i]) * utility::PowersOf3[i];
  new_descriptor += CornerOrientationCount * e_slice_edge_combination.getRank();
  return new_descriptor;
}

static uint32_t getDescriptor(const Cube& cube) {
  uint32_t descriptor = 0;
  for (size_t i = 0; i < Cube::CORNER_LOCATION_ORDER.size() - 1; ++i) {
    const CornerPiece corner = cube[Cube::CORNER_LOCATION_ORDER[i]];
    if (corner.second == Colour::White || corner.second == Colour::Yellow)
      descriptor += utility::pow(3, i);
    else if (corner.third == Colour::White || corner.third == Colour::Yellow)
      descriptor += 2 * utility::pow(3, i);
    // in the remaining case where corner.first == Colour::White || corner.first
    // == Colour::Yellow, we add 0 * utility::pow(3, i) to descriptor, which is
    // equivalent to doing nothing
  }

  Combination<12, 4> e_slice_edge_combination;
  uint8_t i = 0;
  for (uint8_t j = 0; j < Cube::EDGE_LOCATION_ORDER.size(); ++j) {
    const EdgePiece edge = cube[Cube::EDGE_LOCATION_ORDER[j]];
    if (edge.first != Colour::White && edge.first != Colour::Yellow &&
        edge.second != Colour::White && edge.second != Colour::Yellow)
      e_slice_edge_combination[i++] = j;
  }
  // ensure number of E slice edges is 4
  assert(i == e_slice_edge_combination.size());

  descriptor += CornerOrientationCount * e_slice_edge_combination.getRank();

  return descriptor;
}

bool isDominoReduced(const Cube& cube) {
  return areEdgesOriented(cube) && getDescriptor(cube) == SolvedDescriptor;
}

Algorithm solveDominoReduction(Cube cube) {
  static constexpr uint8_t DescriptorBits =
      utility::requiredBits(DescriptorCount);
  static constexpr uint8_t TurnBits =
      utility::requiredBits(PossibleTurns.size());
  static constexpr uint8_t CompressedBits = DescriptorBits + TurnBits;
  static constexpr auto LookupTable =
      utility::PackedBitsArray<CompressedBits, DescriptorCount>::fromRawData({
          // TODO: replace with #embed in c++23
          #include "DominoReductionLookupTable.h"
      });
  static constexpr auto solver = [](uint32_t descriptor) {
    Algorithm alg;
    while (descriptor != SolvedDescriptor) {
      const auto compressed_optimal_move = LookupTable[descriptor];
      alg.push_back(
          Move{PossibleTurns[compressed_optimal_move >> DescriptorBits]});
      descriptor = compressed_optimal_move % (1 << DescriptorBits);
    }
    return alg;
  };

  const Algorithm edge_orientation_solve = solveEdgeOrientation(cube);
  cube.apply(edge_orientation_solve);
  return edge_orientation_solve + solver(getDescriptor(cube));
}

// Test functions

static void testGetDescriptor() {
  if (getDescriptor(Cube{Algorithm::parse("R2")}) != SolvedDescriptor ||
      getDescriptor(Cube{Algorithm::parse("F2")}) != SolvedDescriptor ||
      getDescriptor(Cube{Algorithm::parse("U D'")}) != SolvedDescriptor ||
      getDescriptor(Cube{Algorithm::parse("R")}) !=
          getDescriptor(Cube{Algorithm::parse("R'")}))
    throw std::logic_error("Incorrect descriptor!");
}

static void testApplyTurnForDominoReduction() {
  for (const Turn& turn : PossibleTurns) {
    Algorithm alg;
    alg.push_back(Move{turn});
    if (applyTurn(SolvedDescriptor, turn) != getDescriptor(Cube{alg}))
      throw std::logic_error("Descriptor mismatch!");
  }

  static constexpr size_t Count = 1000;

  for (size_t i = 0; i < Count; ++i) {
    const Algorithm alg = Algorithm::random(20);
    uint32_t descriptor = SolvedDescriptor;
    for (const Move& move : alg)
      descriptor = applyTurn(descriptor, move.getTurn());
    if (descriptor != getDescriptor(Cube{alg}))
      throw std::logic_error("Descriptor mismatch!");
  }
}

static void testSolveDominoReduction() {
  static constexpr size_t count = 1000;

  for (size_t i = 0; i < count; ++i) {
    const Algorithm scramble = Algorithm::random(20);
    Cube cube{scramble};
    const Algorithm solve = solveDominoReduction(cube);
    cube.apply(solve);
    if (!isDominoReduced(cube))
      throw std::logic_error("Domino reduction was not achieved!");
  }
}

static void testStatistics() {
  std::unordered_set<uint32_t> seen_descriptors{};
  seen_descriptors.insert(SolvedDescriptor);

  std::vector<size_t> sizes{};
  std::vector<uint32_t> current{};
  current.push_back(SolvedDescriptor);
  std::vector<uint32_t> next{};

  while (!current.empty()) {
    sizes.push_back(current.size());
    for (const uint32_t& idx : current) {
      for (const Turn& turn : PossibleTurns) {
        const uint32_t next_idx = applyTurn(idx, turn);
        if (seen_descriptors.insert(next_idx).second) next.push_back(next_idx);
      }
    }
    current = next;
    next.clear();
  }
  if (std::accumulate(sizes.begin(), sizes.end(), 0ull) != DescriptorCount)
    throw std::logic_error("Did not encounter every descriptor!");
  if (sizes.size() != 11)
    throw std::logic_error("Some descriptors required more than 10 moves!");
}

void runDominoReductionSolverTests() {
  generateLookupTable<DescriptorCount, PossibleTurns, applyTurn,
                      SolvedDescriptor>(
      "./include/solvers/DominoReductionLookupTable.h");

  testGetDescriptor();
  testApplyTurnForDominoReduction();
  testStatistics();
  testSolveDominoReduction();
}
}  // namespace solvers
