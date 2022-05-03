#include "EdgeCycleSequenceIterator.h"
#include "Utils.h"
#include "Lettering.h"
#include <vector>

namespace blindsolving {
EdgeCycleSequenceIterator::EdgeCycleSequenceIterator(
    const std::vector<std::vector<char>>& cycles,
    const std::vector<bool>& are_even)
    : it(CycleSequenceIterator<char>{cycles}), are_even(are_even) {}

bool EdgeCycleSequenceIterator::operator++() { return ++it; }

std::vector<char> EdgeCycleSequenceIterator::operator*() const {
  std::vector<std::vector<char>> current = *it;
  const std::vector<size_t>& permutation = it.getPermutation();
  for (size_t i = 0; i < current.size(); i++) {
    // add the final cycle-closing target
    current[i].push_back(are_even[permutation[i]] ? current[i][0]
                                                  : flipEdge(current[i][0]));
  }
  return utility::flatten(current);
}
}  // namespace blindsolving
