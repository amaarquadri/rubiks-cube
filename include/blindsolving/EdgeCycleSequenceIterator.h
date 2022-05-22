#pragma once

#include "CycleSequenceIterator.h"
#include <vector>

namespace blindsolving {
class EdgeCycleSequenceIterator {
 private:
  CycleSequenceIterator<char> it;
  const std::vector<bool> are_even;

  /**
   * For any given output of the CycleSequenceIterator, the corresponding output
   * can be modified by inverting any number of its edge cycles (i.e. inverting
   * every edge in that cycle). This vector of booleans tracks which ones are
   * currently being inverted.
   */
  std::vector<bool> are_inverted;

 public:
  EdgeCycleSequenceIterator(const std::vector<std::vector<char>>& cycles,
                            const std::vector<bool>& are_even);

  bool operator++();

  std::vector<char> operator*() const;

  size_t getPeriod() const;

  void reset();
};
}  // namespace blindsolving
