#include "CornerCycleSequenceIterator.h"
#include "CornerRotationAmount.h"
#include "Cube.h"
#include "CycleSequenceIterator.h"
#include "Lettering.h"
#include "Utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <vector>

namespace blindsolving {
CornerCycleSequenceIterator::CornerCycleSequenceIterator(
    const std::vector<std::vector<char>>& cycles,
    const std::vector<CornerRotationAmount>& rotation_amounts)
    : it(CycleSequenceIterator<char>{cycles}),
      rotation_amounts(rotation_amounts),
      modifications(std::vector<CornerRotationAmount>{
          cycles.size(), CornerRotationAmount::None}) {}

bool CornerCycleSequenceIterator::operator++() {
  // attempt to increment the set of CornerRotationAmounts
  for (CornerRotationAmount& rot : modifications) {
    rot = rot + CornerRotationAmount::Clockwise;
    if (rot != CornerRotationAmount::None) return true;
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
      case CornerRotationAmount::None:
        // just add the final cycle-closing target
        current[i].push_back(current[i][0]);
        break;
      case CornerRotationAmount::Clockwise:
        // rotate the last counters[permutations[i]] elements clockwise (which
        // wrap around to the end)
        for (size_t j = current[i].size() - counters[permutation[i]];
             j < current[i].size(); j++)
          current[i][j] = rotateClockwise(current[i][j]);
        // add the final cycle-closing target
        current[i].push_back(rotateClockwise(current[i][0]));
        break;
      case CornerRotationAmount::Counterclockwise:
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
      case CornerRotationAmount::None:
        break;
      case CornerRotationAmount::Clockwise:
        for (char& c : current[i]) c = rotateClockwise(c);
        break;
      case CornerRotationAmount::Counterclockwise:
        for (char& c : current[i]) c = rotateCounterclockwise(c);
        break;
    }
  }
  return utility::flatten(current);
}

size_t CornerCycleSequenceIterator::getPeriod() const {
  assert(rotation_amounts.size() < utility::PowersOf3.size());
  return it.getPeriod() * utility::PowersOf3[rotation_amounts.size()];
}

void CornerCycleSequenceIterator::reset() {
  it.reset();
  std::fill(modifications.begin(), modifications.end(),
            CornerRotationAmount::None);
}
}  // namespace blindsolving
