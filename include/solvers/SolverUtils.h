#pragma once

#include "Algorithm.h"
#include "Face.h"
#include "MathUtils.h"
#include "PackedBitsArray.h"
#include "StaticVector.h"
#include "Turn.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
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

template <auto DescriptorCount, auto PossibleTurns, auto applyTurn,
          auto SolvedDescriptor = 0>
consteval auto getSolver() {
  using Uint = decltype(DescriptorCount);
  static_assert(std::unsigned_integral<Uint>);
  static_assert(DescriptorCount > 0);
  static_assert(std::is_same_v<const std::array<Turn, PossibleTurns.size()>,
                               decltype(PossibleTurns)>);
  static_assert(PossibleTurns.size() > 0);
  static_assert(
      std::is_same_v<Uint (*)(const Uint&, const Turn&), decltype(applyTurn)> ||
      std::is_same_v<Uint (*)(Uint, const Turn&), decltype(applyTurn)>);
  static_assert(std::is_same_v<Uint, decltype(SolvedDescriptor)>);
  static_assert(SolvedDescriptor < DescriptorCount);

  return [](Uint descriptor) {
    static constexpr auto OptimalMoves = []() consteval {
      /** static **/ constexpr Uint UnknownSentinel = DescriptorCount;

      std::array<std::pair<Turn, Uint>, DescriptorCount> optimal_moves{};
      for (size_t i = 0; i < DescriptorCount; ++i)
        optimal_moves[i].second = UnknownSentinel;

      utility::StaticVector<Uint, DescriptorCount> current{};
      current.push_back(SolvedDescriptor);
      utility::StaticVector<Uint, DescriptorCount> next{};

      //  for (size_t i = 0; !current.isEmpty() && i < 1; ++i) {
      while (!current.isEmpty()) {
        for (const Uint& idx : current) {
          for (const Turn& turn : PossibleTurns) {
            const Uint next_idx = applyTurn(idx, turn);
            if (next_idx != SolvedDescriptor &&
                optimal_moves[next_idx].second == UnknownSentinel) {
              optimal_moves[next_idx].first = turn.inv();
              optimal_moves[next_idx].second = idx;
              next.push_back(next_idx);
            }
          }
        }
        current = next;
        next.clear();
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
    ();
    static constexpr uint8_t DescriptorBits =
        utility::requiredBits(DescriptorCount);
    static constexpr auto CompressedOptimalMoves = []() consteval {
      /** static **/ constexpr uint8_t TurnBits =
          utility::requiredBits(PossibleTurns.size());
      /** static **/ constexpr uint8_t CompressedBits =
          DescriptorBits + TurnBits;

      utility::PackedBitsArray<CompressedBits, DescriptorCount>
          compressed_optimal_moves;
      /**
       * CompressedOptimalMoves[SolvedDescriptor] should never be accessed,
       * so is assigned the maximum allowable value to hopefully cause immediate
       * failure if used.
       */
      // TODO: handle case where CompressedBits == 64
      static_assert(CompressedBits < 64);
      compressed_optimal_moves[SolvedDescriptor] = (1ull << CompressedBits) - 1;

      for (Uint i = 0; i < DescriptorCount; ++i) {
        if (i == SolvedDescriptor) continue;
        const size_t possible_turns_index =
            std::find(PossibleTurns.begin(), PossibleTurns.end(),
                      OptimalMoves[i].first) -
            PossibleTurns.begin();
        assert(possible_turns_index != PossibleTurns.size());
        compressed_optimal_moves[i] =
            (possible_turns_index << DescriptorBits) + OptimalMoves[i].second;
      }
      return compressed_optimal_moves;
    }
    ();

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
}  // namespace solvers
