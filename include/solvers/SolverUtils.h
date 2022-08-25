#pragma once

#include "Algorithm.h"
#include "BidirectionalStaticVector.h"
#include "CornerLocation.h"
#include "Cube.h"
#include "EdgeLocation.h"
#include "Face.h"
#include "HeapArray.h"
#include "MathUtils.h"
#include "PackedBitsArray.h"
#include "Turn.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

namespace solvers {
consteval uint8_t getEdgeIndex(const Face& first, const Face& second) {
  const EdgeLocation location{first, second};
  uint8_t idx = std::find(Cube::EdgeLocationOrder.begin(),
                          Cube::EdgeLocationOrder.end(), location) -
                Cube::EdgeLocationOrder.begin();
  if (idx != Cube::EdgeLocationOrder.size()) return idx;

  idx = std::find(Cube::EdgeLocationOrder.begin(),
                  Cube::EdgeLocationOrder.end(), location.flip()) -
        Cube::EdgeLocationOrder.begin();
  assert(idx != Cube::EdgeLocationOrder.size());
  return idx;
}

consteval uint8_t getCornerIndex(const Face& first, const Face& second,
                                 const Face& third) {
  const CornerLocation location{first, second, third};
  uint8_t idx = std::find(Cube::CornerLocationOrder.begin(),
                          Cube::CornerLocationOrder.end(), location) -
                Cube::CornerLocationOrder.begin();
  if (idx != Cube::CornerLocationOrder.size()) return idx;

  idx =
      std::find(Cube::CornerLocationOrder.begin(),
                Cube::CornerLocationOrder.end(), location.rotateClockwise()) -
      Cube::CornerLocationOrder.begin();
  if (idx != Cube::CornerLocationOrder.size()) return idx;

  idx = std::find(Cube::CornerLocationOrder.begin(),
                  Cube::CornerLocationOrder.end(),
                  location.rotateCounterclockwise()) -
        Cube::CornerLocationOrder.begin();
  assert(idx != Cube::CornerLocationOrder.size());
  return idx;
}

constexpr std::array<uint8_t, 4> getEdgeCycle(const Face& face) {
  using enum Face;
  switch (face) {
    case U:
      return {getEdgeIndex(U, B), getEdgeIndex(U, R), getEdgeIndex(U, F),
              getEdgeIndex(U, L)};
    case F:
      return {getEdgeIndex(F, U), getEdgeIndex(F, R), getEdgeIndex(F, D),
              getEdgeIndex(F, L)};
    case R:
      return {getEdgeIndex(R, U), getEdgeIndex(R, B), getEdgeIndex(R, D),
              getEdgeIndex(R, F)};
    case B:
      return {getEdgeIndex(B, U), getEdgeIndex(B, L), getEdgeIndex(B, D),
              getEdgeIndex(B, R)};
    case L:
      return {getEdgeIndex(L, U), getEdgeIndex(L, F), getEdgeIndex(L, D),
              getEdgeIndex(L, B)};
    case D:
      return {getEdgeIndex(D, F), getEdgeIndex(D, R), getEdgeIndex(D, B),
              getEdgeIndex(D, L)};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

/**
 * For the F, R, B, and L faces, the corners will also need to be rotated prior
 * to being cycled if the RotationAmount is Clockwise or Counterclockwise. In
 * this case, index 0 and 2 in the resulting array will be rotated Clockwise,
 * and index 1 and 3 will be rotated Counterclockwise.
 */
constexpr std::array<uint8_t, 4> getCornerCycle(const Face& face) {
  using enum Face;
  switch (face) {
    case U:
      return {getCornerIndex(U, L, B), getCornerIndex(U, B, R),
              getCornerIndex(U, R, F), getCornerIndex(U, F, L)};
    case F:
      return {getCornerIndex(F, L, U), getCornerIndex(F, U, R),
              getCornerIndex(F, R, D), getCornerIndex(F, D, L)};
    case R:
      return {getCornerIndex(R, F, U), getCornerIndex(R, U, B),
              getCornerIndex(R, B, D), getCornerIndex(R, D, F)};
    case B:
      return {getCornerIndex(B, R, U), getCornerIndex(B, U, L),
              getCornerIndex(B, L, D), getCornerIndex(B, D, R)};
    case L:
      return {getCornerIndex(L, B, U), getCornerIndex(L, U, F),
              getCornerIndex(L, F, D), getCornerIndex(L, D, B)};
    case D:
      return {getCornerIndex(D, L, F), getCornerIndex(D, F, R),
              getCornerIndex(D, R, B), getCornerIndex(D, B, L)};
    default:
      throw std::logic_error("Unknown enum value!");
  }
}

/**
 * Transforms the provided data so that any element in data that appears in
 * cycle at index i will be replaced with the element of cycle at index (i +
 * steps) % cycle.size(). Any elements in data that do not appear in cycle will
 * remain unchanged. The data will be sorted before the function terminates.
 */
template <typename T, size_t n, size_t m>
constexpr void cycleValues(std::array<T, n>& data,
                           const std::array<T, m>& cycle,
                           const size_t& steps = 1) {
  for (T& element : data) {
    const size_t cycle_index =
        std::find(cycle.begin(), cycle.end(), element) - cycle.begin();
    if (cycle_index == cycle.size()) continue;
    element = cycle[(cycle_index + steps) % cycle.size()];
  }
  std::sort(data.begin(), data.end());
}

namespace detail {
/**
 * Checks whether the provided template parameters form a syntactically valid
 * (mathematical) group.
 */
template <auto DescriptorCount, auto PossibleTurns, auto applyTurn,
          auto SolvedDescriptor>
struct is_cubing_group {
 private:
  using Uint = decltype(DescriptorCount);
  static constexpr bool are_descriptors_valid =
      std::unsigned_integral<Uint> &&
      std::is_same_v<Uint, decltype(SolvedDescriptor)> && DescriptorCount > 0 &&
      SolvedDescriptor < DescriptorCount;
  static constexpr bool is_possible_turns_valid =
      !PossibleTurns.empty() &&
      std::is_same_v<const std::array<Turn, PossibleTurns.size()>,
                     decltype(PossibleTurns)>;
  static constexpr bool is_apply_turn_valid =
      std::is_same_v<Uint (*)(const Uint&, const Turn&), decltype(applyTurn)> ||
      std::is_same_v<Uint (*)(Uint, const Turn&), decltype(applyTurn)>;

 public:
  static constexpr bool value =
      are_descriptors_valid && is_possible_turns_valid && is_apply_turn_valid;
};

template <auto DescriptorCount, auto PossibleTurns, auto applyTurn,
          auto SolvedDescriptor>
static constexpr bool is_cubing_group_v =
    is_cubing_group<DescriptorCount, PossibleTurns, applyTurn,
                    SolvedDescriptor>::value;

template <auto DescriptorCount, auto PossibleTurns, auto applyTurn,
          auto SolvedDescriptor, bool use_heap>
  requires is_cubing_group_v<DescriptorCount, PossibleTurns, applyTurn,
                             SolvedDescriptor>
static constexpr auto getOptimalMoves() {
  using Uint = decltype(DescriptorCount);

  /** static **/ constexpr Uint UnknownSentinel = DescriptorCount;

  utility::pick_array_t<std::pair<Turn, Uint>, DescriptorCount, use_heap>
      optimal_moves{};
  for (size_t i = 0; i < DescriptorCount; ++i)
    optimal_moves[i].second = UnknownSentinel;

  auto [forward_vector, backward_vector] =
      utility::BidirectionalStaticVector<Uint, DescriptorCount,
                                         use_heap>::make();
  forward_vector.push_back(SolvedDescriptor);

  //  for (size_t i = 0; !forward_vector.isEmpty() && i < 1; ++i) {
  while (!forward_vector.isEmpty()) {
    // iterate through forward_vector and queue next round in
    // backward_vector
    for (const Uint& idx : forward_vector) {
      for (const Turn& turn : PossibleTurns) {
        const Uint next_idx = applyTurn(idx, turn);
        if (next_idx != SolvedDescriptor &&
            optimal_moves[next_idx].second == UnknownSentinel) {
          optimal_moves[next_idx].first = turn.inv();
          optimal_moves[next_idx].second = idx;
          backward_vector.push_back(next_idx);
        }
      }
    }
    if (backward_vector.isEmpty()) break;
    forward_vector.clear();
    // iterate through backward_vector and queue next round in
    // forward_vector
    for (const Uint& idx : backward_vector) {
      for (const Turn& turn : PossibleTurns) {
        const Uint next_idx = applyTurn(idx, turn);
        if (next_idx != SolvedDescriptor &&
            optimal_moves[next_idx].second == UnknownSentinel) {
          optimal_moves[next_idx].first = turn.inv();
          optimal_moves[next_idx].second = idx;
          forward_vector.push_back(next_idx);
        }
      }
    }
    backward_vector.clear();
  }
  assert(optimal_moves[SolvedDescriptor].second == UnknownSentinel);
  assert([&]() {
    for (size_t i = 0; i < optimal_moves.size(); ++i) {
      if (i == SolvedDescriptor) continue;
      if (optimal_moves[i].second == UnknownSentinel) return false;
    }
    return true;
  }());
  return optimal_moves;
}

template <auto DescriptorCount, auto PossibleTurns, auto applyTurn,
          auto SolvedDescriptor, bool use_heap>
  requires is_cubing_group_v<DescriptorCount, PossibleTurns, applyTurn,
                             SolvedDescriptor>
constexpr auto getCompressedOptimalMoves() {
  using Uint = decltype(DescriptorCount);
  /** static **/ constexpr uint8_t DescriptorBits =
      utility::requiredBits(DescriptorCount);
  /** static **/ constexpr uint8_t TurnBits =
      utility::requiredBits(PossibleTurns.size());
  /** static **/ constexpr uint8_t CompressedBits = DescriptorBits + TurnBits;

  const auto optimal_moves =
      detail::getOptimalMoves<DescriptorCount, PossibleTurns, applyTurn,
                              SolvedDescriptor, use_heap>();

  utility::PackedBitsArray<CompressedBits, DescriptorCount, use_heap>
      compressed_optimal_moves;
  /**
   * CompressedOptimalMoves[SolvedDescriptor] should never be accessed,
   * so is assigned the maximum allowable value to hopefully cause immediate
   * failure if used.
   */
  if constexpr (CompressedBits == 64)
    compressed_optimal_moves[SolvedDescriptor] =
        std::numeric_limits<uint64_t>::max();
  else
    compressed_optimal_moves[SolvedDescriptor] = (1ull << CompressedBits) - 1;

  for (Uint i = 0; i < DescriptorCount; ++i) {
    if (i == SolvedDescriptor) continue;
    const size_t possible_turns_index =
        std::find(PossibleTurns.begin(), PossibleTurns.end(),
                  optimal_moves[i].first) -
        PossibleTurns.begin();
    assert(possible_turns_index != PossibleTurns.size());
    compressed_optimal_moves[i] =
        (possible_turns_index << DescriptorBits) + optimal_moves[i].second;
  }

  return compressed_optimal_moves;
}
}  // namespace detail

/**
 * If this crashes during compilation due to a lack of memory, use
 * generateLookupTable and embed the generated header file in the code
 * instead.
 */
template <auto DescriptorCount, auto PossibleTurns, auto applyTurn,
          auto SolvedDescriptor = 0>
  requires detail::is_cubing_group_v<DescriptorCount, PossibleTurns, applyTurn,
                                     SolvedDescriptor>
consteval auto getSolver() {
  using Uint = decltype(DescriptorCount);
  return [](Uint descriptor) {
    static constexpr uint8_t DescriptorBits =
        utility::requiredBits(DescriptorCount);
    static constexpr auto CompressedOptimalMoves =
        detail::getCompressedOptimalMoves<DescriptorCount, PossibleTurns,
                                          applyTurn, SolvedDescriptor, false>();

    Algorithm alg;
    while (descriptor != SolvedDescriptor) {
      const auto compressed_optimal_move = CompressedOptimalMoves[descriptor];
      alg.push_back(
          Move{PossibleTurns[compressed_optimal_move >> DescriptorBits]});
      descriptor = compressed_optimal_move % (1 << DescriptorBits);
    }
    return alg;
  };
}

template <auto DescriptorCount, auto PossibleTurns, auto applyTurn,
          auto SolvedDescriptor = 0>
  requires detail::is_cubing_group_v<DescriptorCount, PossibleTurns, applyTurn,
                                     SolvedDescriptor>
auto generateLookupTable(const std::string& file_name) {
  const auto compressed_optimal_moves =
      detail::getCompressedOptimalMoves<DescriptorCount, PossibleTurns,
                                        applyTurn, SolvedDescriptor, true>();

  std::ofstream data_table_file{file_name};
  for (const uint8_t& element : compressed_optimal_moves.rawData())
    data_table_file << std::to_string(element) << ',';
}
}  // namespace solvers
