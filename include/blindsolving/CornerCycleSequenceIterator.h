#pragma once

#include "CornerRotationAmount.h"
#include "CycleSequenceIterator.h"
#include "Lettering.h"
#include "Utils.h"
#include <algorithm>
#include <vector>

namespace blindsolving {
class CornerCycleSequenceIterator {
 private:
  CycleSequenceIterator<char> it;
  const std::vector<CornerRotationAmount>& rotation_amounts;

  /**
   * For any given output of the CycleSequenceIterator, the corresponding output
   * can be modified by rotating any number of its corner cycles (i.e. rotating
   * every corner in that cycle). This vector of CornerRotationAmounts tracks
   * which ones are currently being rotated.
   */
  std::vector<CornerRotationAmount> modifications;

 public:
  CornerCycleSequenceIterator(
      const std::vector<std::vector<char>>& cycles,
      const std::vector<CornerRotationAmount>& rotation_amounts);

  /**
   * @return True if the iterator was successfully incremented, and false if the
   * iterator overflowed.
   */
  bool operator++();

  std::vector<char> operator*() const;
};
}  // namespace blindsolving
