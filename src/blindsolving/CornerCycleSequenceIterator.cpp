#include "CornerCycleSequenceIterator.h"
#include "CornerRotationAmount.h"
#include "Cube.h"
#include "CycleSequenceIterator.h"
#include "Lettering.h"
#include "Utils.h"
#include <algorithm>
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

  // add the final cycle-closing targets
  const std::vector<size_t>& counters = it.getCounters();
  const std::vector<size_t>& permutation = it.getPermutation();
  for (size_t i = 0; i < current.size(); i++) {
    switch (rotation_amounts[permutation[i]]) {
      case CornerRotationAmount::NONE:
        // just add the final cycle-closing target
        current[i].push_back(current[i][0]);
        break;
      case CornerRotationAmount::CLOCKWISE:
        // rotate the last counters[permutations[i]] elements clockwise (which
        // wrap around to the end)
        for (size_t j = current[i].size() - counters[permutation[i]];
             j < current[i].size(); j++)
          current[i][j] = rotateClockwise(current[i][j]);
        // add the final cycle-closing target
        current[i].push_back(rotateClockwise(current[i][0]));
        break;
      case CornerRotationAmount::COUNTERCLOCKWISE:
        // rotate the last counters[permutations[i]] elements counterclockwise
        // (which wrap around to the end)
        for (size_t j = current[i].size() - counters[permutation[i]];
             j < current[i].size(); j++)
          current[i][j] = rotateCounterclockwise(current[i][j]);
        // add the final cycle-closing target
        current[i].push_back(rotateCounterclockwise(current[i][0]));
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
        for (char& c : current[i]) c = rotateCounterclockwise(c);
        break;
    }
  }
  return utility::flatten(current);
}

size_t CornerCycleSequenceIterator::getPeriod() const {
  static constexpr std::array<size_t, Cube::CORNER_LOCATION_ORDER.size()>
      POWERS_OF_3{1, 3, 9, 27, 81, 243, 729, 2187};
  return it.getPeriod() * POWERS_OF_3[rotation_amounts.size()];
}

void CornerCycleSequenceIterator::reset() {
  it.reset();
  std::fill(modifications.begin(), modifications.end(),
            CornerRotationAmount::NONE);
}
}  // namespace blindsolving
