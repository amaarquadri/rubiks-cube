#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <random>
#include <stdexcept>
#include <utility>

template <size_t n>
class Permutation {
 private:
  std::array<uint8_t, n> values;

  explicit Permutation(std::array<uint8_t, n> values)
      : values(std::move(values)) {}

 public:
  template <typename URNG>
  static Permutation<n> randomPermutation(URNG&& g) {
    if (n < 2) throw std::invalid_argument("Error: n < 2");
    std::array<uint8_t, n> values;
    std::iota(values.begin(), values.end(), 0);
    std::shuffle(values.begin(), values.end(), g);
    return Permutation{values};
  }

  [[nodiscard]] bool isOdd() const {
    std::array<bool, n> visited{};
    bool parity = false;
    for (size_t startingIndex = 0; startingIndex < n; startingIndex++) {
      if (!visited[startingIndex]) {
        visited[startingIndex] = true;
        uint8_t cycle_index = values[startingIndex];
        bool cycle_parity = false;
        while (cycle_index != startingIndex) {
          visited[cycle_index] = true;
          cycle_index = values[cycle_index];
          cycle_parity = !cycle_parity;
        }
        parity ^= cycle_parity;
      }
    }
    return parity;
  }

  void flipParity() { std::swap(values[0], values[1]); }

  template <typename T>
  std::array<T, n> apply(const std::array<T, n>& items) const {
    std::array<T, n> permuted_items;
    for (size_t i = 0; i < n; i++) permuted_items[i] = items[values[i]];
    return permuted_items;
  }
};
