#pragma once

#include "Algorithm.h"
#include "Cube.h"

namespace solvers {
bool isDominoReduced(const Cube& cube);

Algorithm solveDominoReduction(Cube cube);

void runDominoReductionSolverTests();
}  // namespace solvers