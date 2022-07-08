#pragma once

#include "Algorithm.h"
#include "Cube.h"

namespace solvers {
bool isHalfTurnReduced(const Cube& cube);

Algorithm solveHalfTurnReduction(Cube cube);
}  // namespace solvers
