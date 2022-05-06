#pragma once

#include "SolveData.h"
#include <string>
#include <vector>

namespace blindsolving {
using Reconstruction = std::vector<SolveData>;

std::string toStr(const Reconstruction& reconstruction);
}  // namespace blindsolving
