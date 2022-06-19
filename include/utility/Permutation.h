#pragma once

#include "RandomUtils.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <random>
#include <stdexcept>
#include <utility>

template <size_t n>
class Permutation : public std::array<uint8_t, n> {
 public:
  static Permutation<n> randomPermutation() {
    if (n < 2) throw std::invalid_argument("Error: n < 2");
    Permutation<n> permutation;
    std::iota(permutation.begin(), permutation.end(), 0);
    std::shuffle(permutation.begin(), permutation.end(),
                 utility::random_engine);
    return permutation;
  }

  constexpr bool isOdd() const {
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
    std::array<T, n> permuted_items;
    for (size_t i = 0; i < n; ++i) permuted_items[i] = items[(*this)[i]];
    return permuted_items;
  }
};
