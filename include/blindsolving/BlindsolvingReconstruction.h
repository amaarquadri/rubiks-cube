#pragma once

#include "BlindsolvingMove.h"
#include "Cube.h"
#include <string>
#include <vector>

namespace blindsolving {
class BlindsolvingReconstruction : public std::vector<BlindsolvingMove> {
 public:
  void applyTo(Cube& cube) const;

  std::string toStr() const;
};
}  // namespace blindsolving
