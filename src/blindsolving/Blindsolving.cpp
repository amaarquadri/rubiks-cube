#include "Blindsolving.h"
#include "BlindsolvingReconstruction.h"
#include "Cube.h"
#include "Lettering.h"
#include "PLL.h"
#include "Reconstruction.h"
#include "ReconstructionIterator.h"
#include "Utils.h"
#include <stdexcept>
#include <unordered_map>

namespace blindsolving {
static const Algorithm A_ALG =  // NOLINT(cert-err58-cpp)
    Algorithm::parse("M2");
static const Algorithm C_ALG =  // NOLINT(cert-err58-cpp)
    Algorithm::parse("U2 M' U2 M'");
static const Algorithm E_ALG =  // NOLINT(cert-err58-cpp)
    Algorithm::parse("D M' U R2 U' M U R2 U' D' M2");
const std::unordered_map<char, Algorithm> EDGE_ALGS =  // NOLINT(cert-err58-cpp)
    {
        {'A', A_ALG},
        {'B', A_ALG.withSetup("R U R' U'")},
        {'C', C_ALG},
        {'D', A_ALG.withSetup("L' U' L U")},
        {'E', E_ALG},
        {'F', A_ALG.withSetup("U R U'")},
        {'H', A_ALG.withSetup("U' L' U")},
        {'I', A_ALG.withSetup("B' R B")},
        {'J', A_ALG.withSetup("R B' R' B")},
        {'K', A_ALG.withSetup("B' R' B")},
        {'L', A_ALG.withSetup("B' R2 B")},
        {'M', Algorithm::parse("U' M'") * 3 + Algorithm::parse("U' M") +
                  Algorithm::parse("U' M'") * 4},
        {'N', A_ALG.withSetup("U' L U")},
        {'O', E_ALG.inv()},
        {'P', A_ALG.withSetup("U R' U'")},
        {'Q', A_ALG.withSetup("B L' B'")},
        {'R', A_ALG.withSetup("B L2 B'")},
        {'S', A_ALG.withSetup("B L B'")},
        {'T', A_ALG.withSetup("L' B L B'")},
        {'V', A_ALG.withSetup("U R2 U'")},
        {'W', C_ALG.inv()},
        {'Z', A_ALG.withSetup("U' L2 U")},
};

static const Algorithm L_ALG = Algorithm::parse(  // NOLINT(cert-err58-cpp)
    "R U' R' U' R U R' F' R U R' U' R' F R");
const std::unordered_map<char, Algorithm>
    CORNER_ALGS =  // NOLINT(cert-err58-cpp)
    {{'B', Ja_PERM.withSetup("U'")},
     {'C', Y_PERM},
     {'D', Jb_PERM.withSetup("U2")},
     {'E', L_ALG.withSetup("F' D")},
     {'F', Ja_PERM.withSetup("R U'")},
     {'G', Y_PERM.withSetup("R")},
     {'H', L_ALG.withSetup("D")},
     {'I', L_ALG.withSetup("R'")},
     {'J', L_ALG.withSetup("R2")},
     {'K', L_ALG.withSetup("R")},
     {'L', L_ALG},
     {'M', Y_PERM.withSetup("R'")},
     {'O', L_ALG.withSetup("D' R")},
     {'P', L_ALG.withSetup("D'")},
     {'R', Y_PERM.withSetup("F")},
     {'S', Y_PERM.withSetup("D R")},
     {'T', L_ALG.withSetup("D2")},
     {'U', L_ALG.withSetup("F'")},
     {'V', Y_PERM.withSetup("D R2")},
     {'W', Y_PERM.withSetup("R2")},
     {'Z', Y_PERM.withSetup("D' R2")}};

const Algorithm PARITY_ALG =  // NOLINT(cert-err58-cpp)
    A_ALG.withSetup("D' L2 D");

Reconstruction parseSolveAttempt(const Algorithm& moves) {
  size_t consumed = 0;

  // extract all initial CubeRotations
  CubeOrientation orientation = CubeOrientation::identity();
  while (consumed < moves.size() && moves[consumed].isCubeRotation()) {
    orientation *= moves[consumed].getCubeRotation();
    consumed++;
  }
  for (size_t i = consumed + 1; i < moves.size(); i++) {
    if (moves[i].isCubeRotation()) {
      throw std::invalid_argument("CubeRotations can only be at the start!");
    }
  }

  // set up transformations statically
  // TODO: make this constexpr
  static std::unordered_map<char, Cube> edge_alg_transformations = []() {
    std::unordered_map<char, Cube> transformations;
    for (const auto& [chr, alg] : EDGE_ALGS)
      transformations.insert({chr, Cube{alg}});
    return transformations;
  }();
  static std::unordered_map<char, Cube> corner_alg_transformations = []() {
    std::unordered_map<char, Cube> transformations;
    for (const auto& [chr, alg] : CORNER_ALGS)
      transformations.insert({chr, Cube{alg}});
    return transformations;
  }();
  static const Cube parity_alg_transformation{PARITY_ALG};

  Reconstruction reconstruction;
  while (consumed < moves.size()) {
    bool found_match = false;
    SolveData solve_data{};
    Cube test_transformation{};
    for (size_t i = consumed; i < moves.size(); i++) {
      // TODO: check all 3 RotationAmounts for this Move to allow for detecting
      //       BlindSolvingMoves where turns are cancelled
      test_transformation.apply(orientation.apply(moves[i]));

      for (auto [chr, transformation] : edge_alg_transformations) {
        if (test_transformation == transformation) {
          found_match = true;
          solve_data.moves = moves.subAlgorithm(consumed, i + 1);
          solve_data.is_parsed = true;
          solve_data.blindsolving_move.is_parity = false;
          solve_data.blindsolving_move.is_edge = true;
          solve_data.blindsolving_move.alg = chr;
          break;
        }
      }
      for (auto [chr, transformation] : corner_alg_transformations) {
        if (test_transformation == transformation) {
          found_match = true;
          solve_data.moves = moves.subAlgorithm(consumed, i + 1);
          solve_data.is_parsed = true;
          solve_data.blindsolving_move.is_parity = false;
          solve_data.blindsolving_move.is_edge = false;
          solve_data.blindsolving_move.alg = chr;
          break;
        }
      }
      if (test_transformation == parity_alg_transformation) {
        found_match = true;
        solve_data.moves = moves.subAlgorithm(consumed, i + 1);
        // solve_data is initialized to parity anyway, so no need to update here
      }
      if (found_match) {
        reconstruction.push_back(solve_data);
        consumed = i + 1;  // we consumed [0, i] which has a length of (i + 1)
        break;
      }
    }
    if (!found_match) {
      if (reconstruction.empty() || reconstruction.back().is_parsed) {
        reconstruction.emplace_back(Algorithm{});
      }
      reconstruction.back().moves.push_back(moves[consumed]);
      consumed++;
    }
  }
  return reconstruction;
}

/**
 * @return The EdgeLocation where the given EdgePiece belongs in a solved Cube.
 */
EdgeLocation getLocation(const EdgePiece& edge_piece) {
  const EdgePiece flipped_piece = edge_piece.flip();
  for (size_t i = 0; i < Cube::EdgeLocationOrder.size(); i++) {
    if (edge_piece == Cube::StartingEdgePieces[i]) {
      return Cube::EdgeLocationOrder[i];
    } else if (flipped_piece == Cube::StartingEdgePieces[i]) {
      return Cube::EdgeLocationOrder[i].flip();
    }
  }
  throw std::invalid_argument(
      "Could not find where the provided EdgePiece goes! It must be invalid.");
}

/**
 * @return The CornerLocation where the given CornerPiece belongs in a solved
 * Cube.
 */
CornerLocation getLocation(const CornerPiece& corner_piece) {
  const CornerPiece clockwise_piece = corner_piece.rotateClockwise();
  const CornerPiece counterclockwise_piece =
      corner_piece.rotateCounterclockwise();
  for (size_t i = 0; i < Cube::CornerLocationOrder.size(); i++) {
    if (corner_piece == Cube::StartingCornerPieces[i]) {
      return Cube::CornerLocationOrder[i];
    }
    // rotate the resulting CornerLocation in the opposite direction
    else if (clockwise_piece == Cube::StartingCornerPieces[i]) {
      return Cube::CornerLocationOrder[i].rotateCounterclockwise();
    } else if (counterclockwise_piece == Cube::StartingCornerPieces[i]) {
      return Cube::CornerLocationOrder[i].rotateClockwise();
    }
  }
  throw std::invalid_argument(
      "Could not find where the provided CornerPiece goes! It must be "
      "invalid.");
}

ReconstructionIterator getReconstructionIterator(Cube cube) {
  /**
   * Solves the current edge cycle until the buffer is in the correct location
   * (or flipped).
   */
  const auto solve_edge_cycle = [&](std::vector<char>& edge_cycle) {
    EdgeLocation target = getLocation(cube[EDGE_BUFFER]);
    while (target != EDGE_BUFFER && target != EDGE_BUFFER.flip()) {
      edge_cycle.push_back(EDGE_LETTERING.at(target));
      cube.cycleEdges<2>({EDGE_BUFFER, target});
      target = getLocation(cube[EDGE_BUFFER]);
    }
  };

  std::vector<char> first_edge_cycle;
  solve_edge_cycle(first_edge_cycle);

  std::vector<std::vector<char>> edge_cycles;
  std::vector<bool> are_even;
  while (true) {
    bool found_unsolved_edge = false;
    std::vector<char> edge_cycle;
    for (size_t i = 0; i < Cube::EdgeLocationOrder.size(); i++) {
      if (Cube::EdgeLocationOrder[i] == EDGE_BUFFER)
        continue;  // skip the buffer piece itself
      if (cube.getEdgeByIndex(i) != Cube::StartingEdgePieces[i]) {
        found_unsolved_edge = true;
        edge_cycle.push_back(EDGE_LETTERING.at(Cube::EdgeLocationOrder[i]));
        cube.cycleEdges<2>({EDGE_BUFFER, Cube::EdgeLocationOrder[i]});
        break;
      }
    }
    if (!found_unsolved_edge) break;
    solve_edge_cycle(edge_cycle);
    are_even.push_back(edge_cycle.back() == edge_cycle.front());
    edge_cycle.pop_back();
    edge_cycles.push_back(edge_cycle);
  }

  const bool has_parity = [&]() {
    size_t count = first_edge_cycle.size() + edge_cycles.size();
    for (const auto& cycle : edge_cycles) count += cycle.size();
    return count % 2 == 1;
  }();

  /**
   * Solves the current corner cycle until the buffer is in the correct location
   * (or rotated).
   */
  const auto solve_corner_cycle = [&](std::vector<char>& corner_cycle) {
    CornerLocation target = getLocation(cube[CORNER_BUFFER]);
    while (target != CORNER_BUFFER &&
           target != CORNER_BUFFER.rotateClockwise() &&
           target != CORNER_BUFFER.rotateCounterclockwise()) {
      corner_cycle.push_back(CORNER_LETTERING.at(target));
      cube.cycleCorners<2>({CORNER_BUFFER, target});
      target = getLocation(cube[CORNER_BUFFER]);
    }
  };

  std::vector<char> first_corner_cycle;
  solve_corner_cycle(first_corner_cycle);

  std::vector<std::vector<char>> corner_cycles;
  std::vector<CornerRotationAmount> rotation_amounts;
  while (true) {
    bool found_unsolved_corner = false;
    std::vector<char> corner_cycle;
    for (size_t i = 0; i < Cube::CornerLocationOrder.size(); i++) {
      if (Cube::CornerLocationOrder[i] == CORNER_BUFFER)
        continue;  // skip the buffer piece itself
      if (cube.getCornerByIndex(i) != Cube::StartingCornerPieces[i]) {
        found_unsolved_corner = true;
        corner_cycle.push_back(
            CORNER_LETTERING.at(Cube::CornerLocationOrder[i]));
        cube.cycleCorners<2>({CORNER_BUFFER, Cube::CornerLocationOrder[i]});
        break;
      }
    }
    if (!found_unsolved_corner) break;
    solve_corner_cycle(corner_cycle);
    if (corner_cycle.back() == corner_cycle.front())
      rotation_amounts.push_back(CornerRotationAmount::None);
    else if (corner_cycle.back() == rotateClockwise(corner_cycle.front()))
      rotation_amounts.push_back(CornerRotationAmount::Clockwise);
    else if (corner_cycle.back() ==
             rotateCounterclockwise(corner_cycle.front()))
      rotation_amounts.push_back(CornerRotationAmount::Counterclockwise);
    else
      throw std::logic_error(
          "Last target in cycle doesn't match first target!");
    corner_cycle.pop_back();
    corner_cycles.push_back(corner_cycle);
  }

  return ReconstructionIterator{
      first_edge_cycle,   edge_cycles,   are_even,        has_parity,
      first_corner_cycle, corner_cycles, rotation_amounts};
}

std::vector<std::pair<BlindsolvingReconstruction, size_t>>
getBestReconstructions(const Reconstruction& solve, ReconstructionIterator& it,
                       const size_t& max) {
  /**
   * Used to mark unparsed components of the parsed solve to differentiate it
   * from parsed components when computing Levenshtein edit distances.
   */
  static constexpr BlindsolvingMove UNPARSED_SENTINEL{true, ' '};

  const size_t period = it.getPeriod();
  it.reset();  // ensures that all possible reconstructions are checked

  const BlindsolvingReconstruction baseline = [&]() {
    BlindsolvingReconstruction baseline;
    for (const SolveData& solve_data : solve) {
      if (solve_data.is_parsed)
        baseline.push_back(solve_data.blindsolving_move);
      else
        baseline.push_back(UNPARSED_SENTINEL);
    }
    return baseline;
  }();

  const auto query_iterator = [&]() {
    std::pair<BlindsolvingReconstruction, size_t> reconstruction;
    reconstruction.first = *it;
    reconstruction.second =
        utility::levEditDistance(baseline, reconstruction.first);
    return reconstruction;
  };

  std::vector<std::pair<BlindsolvingReconstruction, size_t>>
      best_reconstructions;
  for (size_t i = 0; i < std::min(period, max); ++i) {
    best_reconstructions.push_back(query_iterator());
    ++it;
  }

  const auto comparator =
      [](const std::pair<BlindsolvingReconstruction, size_t>& first,
         const std::pair<BlindsolvingReconstruction, size_t>& second) {
        return first.second < second.second;
      };
  std::sort(best_reconstructions.begin(), best_reconstructions.end(),
            comparator);

  if (period == max) return best_reconstructions;

  do {
    std::pair<BlindsolvingReconstruction, size_t> reconstruction =
        query_iterator();
    const size_t index = std::upper_bound(best_reconstructions.begin(),
                                          best_reconstructions.end(),
                                          reconstruction, comparator) -
                         best_reconstructions.begin();
    if (index < max) {
      for (size_t i = max - 1; i > index; i--)
        best_reconstructions[i] = best_reconstructions[i - 1];
      best_reconstructions[index] = reconstruction;
    }
  } while (++it);
  return best_reconstructions;
}
}  // namespace blindsolving
