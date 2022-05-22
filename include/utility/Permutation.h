#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <random>
#include <utility>

template <int n>
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
    for (size_t i = 0; i < n; i++) values[i] = i;
    std::shuffle(values.begin(), values.end(), g);
    return Permutation{values};
  }

  [[nodiscard]] bool isOdd() const {
    std::array<bool, n> visited{};
    bool parity = false;
    for (int startingIndex = 0; startingIndex < n; startingIndex++) {
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

  void flipParity() {
    uint8_t temp = values[0];
    values[0] = values[1];
    values[1] = temp;
  }

  template <typename T>
  std::array<T, n> apply(const std::array<T, n>& items) {
    std::array<T, n> permutedItems;
    for (size_t i = 0; i < n; i++) permutedItems[i] = items[values[i]];
    return permutedItems;
  }
};
