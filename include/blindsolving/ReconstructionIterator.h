#pragma once

#include "BlindsolvingMove.h"
#include "CornerCycleSequenceIterator.h"
#include "CornerRotationAmount.h"
#include "Cube.h"
#include "CycleSequenceIterator.h"
#include "EdgeCycleSequenceIterator.h"
#include <vector>

namespace blindsolving {
class ReconstructionIterator {
 private:
  // arbitrary first reconstruction
  const std::vector<char> first_edge_cycle;
  EdgeCycleSequenceIterator edge_iterator;
  const bool has_parity;
  const std::vector<char> first_corner_cycle;
  CornerCycleSequenceIterator corner_iterator;
  const size_t length;

 public:
  ReconstructionIterator(
      const std::vector<char>& first_edge_cycle,
      const std::vector<std::vector<char>>& edge_cycles,
      const std::vector<bool>& are_even, const bool& has_parity,
      const std::vector<char>& first_corner_cycle,
      const std::vector<std::vector<char>>& corner_cycles,
      const std::vector<CornerRotationAmount>& rotation_amounts);

  bool operator++();

  std::vector<BlindsolvingMove> operator*() const;

  size_t getPeriod() const;

  void reset();
};
}  // namespace blindsolving
