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
  static_assert(n <= 20);  // 21! does not fit into a 64-bit unsigned integer

 private:
  /**
   * An std::array containing {(n - 1)!, (n - 2)!, ..., 2!, 1!, 0!}.
   */
  static constexpr std::array<size_t, n> FactorialCoefficients = []() {
    std::array<size_t, n> factorials;
    factorials.back() = 1;  // zero factorial
    for (uint8_t i = 1; i < n; ++i)
      factorials[n - 1 - i] = i * factorials[n - i];  // i factorial
    return factorials;
  }();

  static constexpr size_t n_factorial = n * FactorialCoefficients[0];

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

  constexpr size_t getRank() const {
    assert(isValid());
    size_t rank = 0;
    for (size_t i = 0; i < n; ++i) {
      const uint8_t d =
          (*this)[i] - std::count_if(this->begin(), this->begin() + i,
                                     [&](const uint8_t& value) {
                                       return value < (*this)[i];
                                     });
      rank += FactorialCoefficients[i] * d;
    }

    return rank;
  }

  static constexpr Permutation<n> parseRank(size_t rank) {
    assert(rank < n_factorial);
    Permutation<n> permutation;
    std::array<bool, n>
        used_elements{};  // used_elements[i] is true if i is in permutation
    for (size_t i = 0; i < n; ++i) {
      // remainder and integer quotient will be computed via a single
      // instruction
      uint8_t d = rank / FactorialCoefficients[i];
      rank = rank % FactorialCoefficients[i];

      for (size_t j = 0; j <= d; ++j) {
        assert(j < n);
        if (used_elements[j]) ++d;
      }
      assert(d < n);
      permutation[i] = d;
      used_elements[d] = true;
    }
    assert(permutation.isValid());
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

  /**
   * @return False if and only if the permutation overflowed.
   */
  constexpr bool nextPermutation() {
    assert(isValid());
    return std::next_permutation(this->begin(), this->end());
  }

  /**
   * @return False if and only if the permutation underflowed.
   */
  constexpr bool prevPermutation() {
    assert(isValid());
    return std::prev_permutation(this->begin(), this->end());
  }

  /**
   * Computes the relative rank of this permutation amongst all permutations
   * with the same parity. The result is in the range [0, n! / 2).
   */
  constexpr size_t getParityRank() { return getRank() / 2; }

  /**
   * Computes the permutation with the specified parity_rank and parity.
   */
  static constexpr Permutation<n> parseParityRank(const size_t& parity_rank,
                                                  const bool& is_odd) {
    assert(parity_rank < n_factorial / 2);
    /**
     * We don't know if 2 * parity_rank or 2 * parity_rank + 1 is the rank with
     * the desired parity, but we know that it must be one of them. Thus, we can
     * simply compute the permutation for 2 * parity_rank and take the next
     * permutation if the parity is incorrect.
     */
    Permutation<n> permutation = parseRank(2 * parity_rank);
    if (permutation.isOdd() != is_odd) {
      [[maybe_unused]] const bool overflowed = permutation.nextPermutation();
      assert(!overflowed);
    }
    return permutation;
  }

  template <typename T>
  constexpr std::array<T, n> apply(const std::array<T, n>& items) const {
    assert(isValid());
    std::array<T, n> permuted_items;
    for (size_t i = 0; i < n; ++i) permuted_items[i] = items[(*this)[i]];
    return permuted_items;
  }
};
