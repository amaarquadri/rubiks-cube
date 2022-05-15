#include "CornerCycleSequenceIterator.h"
#include "CornerRotationAmount.h"
#include "CycleSequenceIterator.h"
#include "Lettering.h"
#include "Utils.h"
#include <array>
#include <vector>

namespace blindsolving {
CornerCycleSequenceIterator::CornerCycleSequenceIterator(
    const std::vector<std::vector<char>>& cycles,
    const std::vector<CornerRotationAmount>& rotation_amounts)
    : it(CycleSequenceIterator<char>{cycles}),
      rotation_amounts(rotation_amounts),
      modifications(std::vector<CornerRotationAmount>{
          cycles.size(), CornerRotationAmount::NONE}) {}

bool CornerCycleSequenceIterator::operator++() {
  // attempt to increment the set of CornerRotationAmounts
  for (CornerRotationAmount& rot : modifications) {
    rot = rot + CornerRotationAmount::CLOCKWISE;
    if (rot != CornerRotationAmount::NONE) return true;
  }
  // otherwise, increment the parent iterator
  return ++it;
}

std::vector<char> CornerCycleSequenceIterator::operator*() const {
  std::vector<std::vector<char>> current = *it;
  const std::vector<size_t>& permutation = it.getPermutation();
  for (size_t i = 0; i < current.size(); i++) {
    // add the final cycle-closing target
    switch (rotation_amounts[permutation[i]]) {
      case CornerRotationAmount::NONE:
        current[i].push_back(current[i][0]);
        break;
      case CornerRotationAmount::CLOCKWISE:
        current[i].push_back(rotateClockwise(current[i][0]));
        break;
      case CornerRotationAmount::COUNTERCLOCKWISE:
        current[i].push_back(rotateCounterClockwise(current[i][0]));
        break;
      default:
        throw std::logic_error("Unknown enum value!");
    }
  }
  // modify the correct cycles
  for (size_t i = 0; i < modifications.size(); i++) {
    switch (modifications[i]) {
      case CornerRotationAmount::NONE:
        break;
      case CornerRotationAmount::CLOCKWISE:
        for (char& c : current[i]) c = rotateClockwise(c);
        break;
      case CornerRotationAmount::COUNTERCLOCKWISE:
        for (char& c : current[i]) c = rotateCounterClockwise(c);
        break;
    }
  }
  return utility::flatten(current);
}

size_t CornerCycleSequenceIterator::getPeriod() const {
  static constexpr std::array<size_t, 8> POWERS_OF_3{1,  3,   9,   27,
                                                     81, 243, 729, 2187};
  return it.getPeriod() * POWERS_OF_3[rotation_amounts.size()];
}
}  // namespace blindsolving
