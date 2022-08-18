#pragma once

#include "Algorithm.h"
#include "Cube.h"

namespace solvers {
bool isHalfTurnReduced(const Cube& cube);

Algorithm solveHalfTurnReduction(Cube cube);

void runHalfTurnReductionSolverTests();
}  // namespace solvers
