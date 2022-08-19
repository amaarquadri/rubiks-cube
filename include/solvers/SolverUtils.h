#pragma once

#include "Algorithm.h"
#include "BidirectionalStaticVector.h"
#include "CornerLocation.h"
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
constexpr std::array<uint8_t, 4> getEdgeCycle(const Face& face) {
  const auto get_idx = [](const Face& first, const Face& second) consteval {
    const EdgeLocation location{first, second};
    uint8_t idx = std::find(Cube::EDGE_LOCATION_ORDER.begin(),
                            Cube::EDGE_LOCATION_ORDER.end(), location) -
                  Cube::EDGE_LOCATION_ORDER.begin();
    if (idx != Cube::EDGE_LOCATION_ORDER.size()) return idx;

    idx = std::find(Cube::EDGE_LOCATION_ORDER.begin(),
                    Cube::EDGE_LOCATION_ORDER.end(), location.flip()) -
          Cube::EDGE_LOCATION_ORDER.begin();
    assert(idx != Cube::EDGE_LOCATION_ORDER.size());
    return idx;
  };
  using enum Face;
  switch (face) {
    case U:
      return {get_idx(U, B), get_idx(U, R), get_idx(U, F), get_idx(U, L)};
    case F:
      return {get_idx(F, U), get_idx(F, R), get_idx(F, D), get_idx(F, L)};
    case R:
      return {get_idx(R, U), get_idx(R, B), get_idx(R, D), get_idx(R, F)};
    case B:
      return {get_idx(B, U), get_idx(B, L), get_idx(B, D), get_idx(B, R)};
    case L:
      return {get_idx(L, U), get_idx(L, F), get_idx(L, D), get_idx(L, B)};
    case D:
      return {get_idx(D, F), get_idx(D, R), get_idx(D, B), get_idx(D, L)};
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
  const auto get_idx =
      [](const Face& first, const Face& second, const Face& third) consteval {
    const CornerLocation location{first, second, third};
    uint8_t idx = std::find(Cube::CORNER_LOCATION_ORDER.begin(),
                            Cube::CORNER_LOCATION_ORDER.end(), location) -
                  Cube::CORNER_LOCATION_ORDER.begin();
    if (idx != Cube::CORNER_LOCATION_ORDER.size()) return idx;

    idx = std::find(Cube::CORNER_LOCATION_ORDER.begin(),
                    Cube::CORNER_LOCATION_ORDER.end(),
                    location.rotateClockwise()) -
          Cube::CORNER_LOCATION_ORDER.begin();
    if (idx != Cube::CORNER_LOCATION_ORDER.size()) return idx;

    idx = std::find(Cube::CORNER_LOCATION_ORDER.begin(),
                    Cube::CORNER_LOCATION_ORDER.end(),
                    location.rotateCounterclockwise()) -
          Cube::CORNER_LOCATION_ORDER.begin();
    assert(idx != Cube::CORNER_LOCATION_ORDER.size());
    return idx;
  };
  using enum Face;
  switch (face) {
    case U:
      return {get_idx(U, L, B), get_idx(U, B, R), get_idx(U, R, F),
              get_idx(U, F, L)};
    case F:
      return {get_idx(F, L, U), get_idx(F, U, R), get_idx(F, R, D),
              get_idx(F, D, L)};
    case R:
      return {get_idx(R, F, U), get_idx(R, U, B), get_idx(R, B, D),
              get_idx(R, D, F)};
    case B:
      return {get_idx(B, R, U), get_idx(B, U, L), get_idx(B, L, D),
              get_idx(B, D, R)};
    case L:
      return {get_idx(L, B, U), get_idx(L, U, F), get_idx(L, F, D),
              get_idx(L, D, B)};
    case D:
      return {get_idx(D, L, F), get_idx(D, F, R), get_idx(D, R, B),
              get_idx(D, B, L)};
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
