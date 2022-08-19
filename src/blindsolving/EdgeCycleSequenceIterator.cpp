#include "EdgeCycleSequenceIterator.h"
#include "Lettering.h"
#include "Utils.h"
#include <algorithm>
#include <utility>
#include <vector>

namespace blindsolving {
EdgeCycleSequenceIterator::EdgeCycleSequenceIterator(
    const std::vector<std::vector<char>>& cycles, std::vector<bool> are_even)
    : it(CycleSequenceIterator<char>{cycles}),
      are_even(std::move(are_even)),
      are_inverted(std::vector<bool>(cycles.size())) {}

bool EdgeCycleSequenceIterator::operator++() {
  // attempt to increment the set of booleans
  for (auto&& b : are_inverted) {  // must use auto&& so we can write to b
    if (b)
      b = false;
    else {
      b = true;
      return true;
    }
  }
  // otherwise, increment the parent iterator
  return ++it;
}

std::vector<char> EdgeCycleSequenceIterator::operator*() const {
  std::vector<std::vector<char>> current = *it;

  // perform modifications
  const std::vector<size_t>& counters = it.getCounters();
  const std::vector<size_t>& permutation = it.getPermutation();
  for (size_t i = 0; i < current.size(); i++) {
    if (are_even[permutation[i]])
      // just add the final cycle-closing target
      current[i].push_back(current[i][0]);
    else {
      // invert the last counters[permutations[i]] elements (which wrap around
      // to the end)
      for (size_t j = current[i].size() - counters[permutation[i]];
           j < current[i].size(); j++)
        current[i][j] = flipEdge(current[i][j]);
      // add the final cycle-closing target
      current[i].push_back(flipEdge(current[i][0]));
    }
  }
  // invert the correct cycles
  for (size_t i = 0; i < are_inverted.size(); i++)
    if (are_inverted[i])
      for (char& c : current[i]) c = flipEdge(c);
  return utility::flatten(current);
}

size_t EdgeCycleSequenceIterator::getPeriod() const {
  // extra factor of 2 to the power of the number of cycles
  return it.getPeriod() * (1 << are_even.size());
}

void EdgeCycleSequenceIterator::reset() {
  it.reset();
  std::fill(are_inverted.begin(), are_inverted.end(), false);
}
}  // namespace blindsolving
