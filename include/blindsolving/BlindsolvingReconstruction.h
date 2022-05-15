#pragma once

#include "BlindsolvingMove.h"
#include <string>
#include <vector>

namespace blindsolving {
using BlindsolvingReconstruction = std::vector<BlindsolvingMove>;

std::string toStr(const BlindsolvingReconstruction& reconstruction);
}  // namespace blindsolving
