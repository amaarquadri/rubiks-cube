#include "CornerCycleSequenceIterator.h"
#include "CornerRotationAmount.h"
#include "CycleSequenceIterator.h"
#include "Lettering.h"
#include "Utils.h"
#include <vector>

namespace blindsolving {
CornerCycleSequenceIterator::CornerCycleSequenceIterator(
    const std::vector<std::vector<char>>& cycles,
    const std::vector<CornerRotationAmount>& rotation_amounts)
    : it(CycleSequenceIterator<char>{cycles}),
      rotation_amounts(rotation_amounts) {}

bool CornerCycleSequenceIterator::operator++() { return ++it; }

std::vector<char> CornerCycleSequenceIterator::operator*() const {
  std::vector<std::vector<char>> current = *it;
  const std::vector<size_t>& permutation = it.getPermutation();
  for (size_t i = 0; i < current.size(); i++) {
    // add the final cycle-closing target
    switch (rotation_amounts[permutation[i]]) {
      case NONE:
        current[i].push_back(current[i][0]);
        break;
      case CLOCKWISE:
        current[i].push_back(rotateClockwise(current[i][0]));
        break;
      case COUNTERCLOCKWISE:
        current[i].push_back(rotateCounterClockwise(current[i][0]));
        break;
      default:
        throw std::logic_error("Unknown enum value!");
    }
  }
  return utility::flatten(current);
}
}  // namespace blindsolving
