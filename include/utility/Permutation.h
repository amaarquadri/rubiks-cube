#pragma once

#include "RandomUtils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
#include <random>
#include <stdexcept>
#include <utility>

template <size_t n>
class Permutation : public std::array<uint8_t, n> {
  static_assert(n >= 2);
  static_assert(n <= (1 << 8));

 public:
  constexpr bool isValid() const {
    std::array<bool, n> found{};
    for (const uint8_t& value : (*this)) {
      if (value >= n || found[value]) return false;
      found[value] = true;
    }
    return std::all_of(found.begin(), found.end(), std::identity{});
  }

  static Permutation<n> randomPermutation() {
    Permutation<n> permutation;
    std::iota(permutation.begin(), permutation.end(), 0);
    std::shuffle(permutation.begin(), permutation.end(),
                 utility::random_engine);
    return permutation;
  }

  constexpr bool isOdd() const {
    assert(isValid());
    std::array<bool, n> visited{};
    bool parity = false;
    for (size_t starting_index = 0; starting_index < n; ++starting_index) {
      if (!visited[starting_index]) {
        visited[starting_index] = true;
        uint8_t cycle_index = (*this)[starting_index];
        while (cycle_index != starting_index) {
          visited[cycle_index] = true;
          cycle_index = (*this)[cycle_index];
          parity = !parity;
        }
      }
    }
    return parity;
  }

  constexpr void flipParity() { std::swap(this->front(), (*this)[1]); }

  template <typename T>
  constexpr std::array<T, n> apply(const std::array<T, n>& items) const {
    assert(isValid());
    std::array<T, n> permuted_items;
    for (size_t i = 0; i < n; ++i) permuted_items[i] = items[(*this)[i]];
    return permuted_items;
  }
};
