#pragma once

#include "Lettering.h"
#include "MathUtils.h"
#include "Utils.h"
#include <algorithm>
#include <numeric>
#include <vector>

namespace blindsolving {
/**
 * Iterates over all possible permutations of a set of cycles where each cycle
 * can start at any arbitrary location.
 */
template <typename T>
class CycleSequenceIterator {
 private:
  const std::vector<std::vector<T>> cycles;
  std::vector<size_t> permutation;
  std::vector<size_t>
      counters;  // counters[i] is the counter for cycles[permutation[i]]
  mutable std::vector<std::vector<T>> current;
  mutable bool is_current_valid = false;

 public:
  CycleSequenceIterator(const std::vector<std::vector<T>>& cycles)
      : cycles(cycles), current(std::vector<std::vector<T>>{cycles.size()}) {
    permutation.reserve(cycles.size());
    counters.reserve(cycles.size());
    const size_t max_cycle_length =
        std::max_element(
            cycles.begin(), cycles.end(),
            [](const std::vector<T>& first, const std::vector<T>& second) {
              return first.size() < second.size();
            })
            ->size();
    for (size_t i = 0; i < cycles.size(); i++) {
      if (cycles[i].empty()) throw std::invalid_argument("Empty cycle!");
      permutation.push_back(i);
      counters.push_back(0);
      current[i].reserve(max_cycle_length);
    }
  }

  /**
   * @return True if the iterator was successfully incremented, and false if the
   * iterator overflowed.
   */
  bool operator++() {
    is_current_valid = false;

    // increment the counters
    for (const auto& cycle_index : permutation) {
      counters[cycle_index]++;
      if (counters[cycle_index] == cycles[cycle_index].size()) {
        counters[cycle_index] = 0;
      } else
        return true;
    }

    // the counters overflowed and we need to increment the permutation
    return std::next_permutation(permutation.begin(), permutation.end());
  }

  std::vector<std::vector<T>> operator*() const {
    if (!is_current_valid) {
      for (size_t i = 0; i < cycles.size(); i++) {
        const size_t cycle_index = permutation[i];
        current[i].clear();
        const auto& cycle = cycles[cycle_index];
        const auto& offset = counters[cycle_index];
        for (size_t j = 0; j < cycle.size(); j++)
          current[i].push_back(cycle[(j + offset) % cycle.size()]);
      }
      is_current_valid = true;
    }
    return current;
  }

  /**
   * Computes the period of this iterator (i.e. the number of unique outputs
   * before overflow and repetition).
   */
  size_t getPeriod() const {
    size_t period = utility::factorial(cycles.size());
    for (const auto& cycle : cycles) period *= cycle.size();
    return period;
  }

  void reset() {
    std::iota(permutation.begin(), permutation.end(), 0);
    std::fill(counters.begin(), counters.end(), 0);
    is_current_valid = false;
  }

  const std::vector<size_t>& getPermutation() const { return permutation; }
};
}  // namespace blindsolving
