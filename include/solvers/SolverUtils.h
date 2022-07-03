#pragma once

#include "Face.h"
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

template <std::unsigned_integral Uint, Uint DescriptorCount, size_t n>
constexpr std::array<std::pair<Turn, Uint>, DescriptorCount> getOptimalMoves(
    const std::array<Turn, n>& possible_turns,
    Uint (*apply_turn)(const Uint&, const Turn&),
    const Uint& solved_descriptor = 0) {
  constexpr Uint UnknownSentinel = DescriptorCount;

  std::array<std::pair<Turn, Uint>, DescriptorCount> optimal_moves{};
  for (size_t i = 0; i < DescriptorCount; ++i)
    optimal_moves[i].second = UnknownSentinel;

  utility::StaticVector<Uint, DescriptorCount> current{};
  current.push_back(solved_descriptor);
  utility::StaticVector<Uint, DescriptorCount> next{};

  while (!current.isEmpty()) {
    for (const Uint& idx : current) {
      for (const Turn& turn : possible_turns) {
        const Uint next_idx = apply_turn(idx, turn);
        if (next_idx != solved_descriptor &&
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
  assert(optimal_moves[solved_descriptor].second == UnknownSentinel);
  assert([&]() {
    for (size_t i = 0; i < optimal_moves.size(); ++i) {
      if (i == solved_descriptor) continue;
      if (optimal_moves[i].second == UnknownSentinel) return false;
    }
    return true;
  }());
  return optimal_moves;
}
}  // namespace solvers
