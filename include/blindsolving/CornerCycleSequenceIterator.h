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
