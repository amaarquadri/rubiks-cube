#pragma once

#include "SolveData.h"
#include <string>
#include <vector>

namespace blindsolving {
class Reconstruction : public std::vector<SolveData> {
 public:
  [[nodiscard]] std::string toStr() const;
};
}  // namespace blindsolving
