#pragma once

#include "CycleSequenceIterator.h"
#include <vector>

namespace blindsolving {
class EdgeCycleSequenceIterator {
 private:
  CycleSequenceIterator<char> it;
  const std::vector<bool>& are_even;

 public:
  EdgeCycleSequenceIterator(const std::vector<std::vector<char>>& cycles,
                            const std::vector<bool>& are_even);

  bool operator++();

  std::vector<char> operator*() const;
};
}  // namespace blindsolving
