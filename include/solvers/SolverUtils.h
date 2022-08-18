#pragma once

#include "Algorithm.h"
#include "BidirectionalStaticVector.h"
#include "Face.h"
#include "MathUtils.h"
#include "PackedBitsArray.h"
#include "Turn.h"
#include "heap_array.h"
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
  using enum Face;
  switch (face) {
    case U:
      return {0, 1, 2, 3};
    case F:
      return {2, 4, 8, 7};
    case R:
      return {1, 5, 9, 4};
    case B:
      return {0, 6, 10, 5};
    case L:
      return {3, 7, 11, 6};
    case D:
      return {8, 9, 10, 11};
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
      return {0, 1, 2, 3};
    case F:
      return {3, 2, 5, 4};
    case R:
      return {2, 1, 6, 5};
    case B:
      return {1, 0, 7, 6};
    case L:
      return {0, 3, 4, 7};
    case D:
      return {4, 5, 6, 7};
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
