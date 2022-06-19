#pragma once

#include "MathUtils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

template <size_t n, size_t k>
class Combination : public std::array<uint8_t, k> {
 public:
  constexpr bool isValid() const {
    return std::all_of(this->begin(), this->end(),
                       [](const uint8_t& choice) { return choice < n; }) &&
           std::is_sorted(this->begin(), this->end()) &&
           std::adjacent_find(this->begin(), this->end()) == this->end();
  }

  /**
   * @return The lexicographical rank of this combination among all
   * possibilities.
   */
  constexpr size_t getRank() const {
    assert(isValid());
    size_t rank = 0;
    size_t last_choice_plus_one = 0;
    for (size_t i = 0; i < k; ++i) {
      const uint8_t& choice = (*this)[i];
      for (size_t j = last_choice_plus_one; j < choice;
           ++j)  // for every element not chosen
        rank += utility::nChooseK(
            n - j - 1, k - i - 1);  // spots left (not including j) choose
                                    // elements left (not including i)

      last_choice_plus_one = choice + 1;
    }
    return rank;
  }

  /**
   * @return The Combination<n, k> with the given lexicographical rank.
   */
  static constexpr Combination<n, k> parseRank(size_t rank) {
    assert(rank < utility::nChooseK(n, k));
    Combination<n, k> combination;
    size_t candidate_choice = 0;
    for (size_t i = 0; i < k; ++i) {
      const auto get_elements_to_skip = [&]() {
        return utility::nChooseK(n - candidate_choice - 1, k - i - 1);
      };
      size_t elements_to_skip = get_elements_to_skip();
      while (rank >= elements_to_skip) {
        rank -= elements_to_skip;
        ++candidate_choice;
        elements_to_skip = get_elements_to_skip();
      }
      combination[i] = candidate_choice;
      ++candidate_choice;
    }
    return combination;
  }
};

/// Test cases

static_assert(Combination<12, 4>({0, 1, 2, 3}).getRank() == 0);
static_assert(Combination<12, 4>({0, 1, 2, 4}).getRank() == 1);
static_assert(Combination<12, 4>({1, 2, 3, 4}).getRank() ==
              utility::nChooseK(11, 3));
static_assert(Combination<12, 4>({2, 3, 4, 5}).getRank() ==
              utility::nChooseK(11, 3) + utility::nChooseK(10, 3));
static_assert(Combination<12, 4>({1, 3, 4, 5}).getRank() ==
              utility::nChooseK(11, 3) + utility::nChooseK(9, 2));
static_assert(Combination<12, 4>({4, 5, 6, 7}).getRank() ==
              utility::nChooseK(11, 3) + utility::nChooseK(10, 3) +
                  utility::nChooseK(9, 3) + utility::nChooseK(8, 3));
static_assert(Combination<12, 4>({8, 9, 10, 11}).getRank() ==
              utility::nChooseK(12, 4) - 1);
static_assert(Combination<12, 4>::parseRank(1).getRank() == 1);
static_assert(Combination<12, 4>::parseRank(69).getRank() == 69);
static_assert(Combination<12, 4>::parseRank(420).getRank() == 420);
