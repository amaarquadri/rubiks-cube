#pragma once

#include "Algorithm.h"
#include "Cube.h"

namespace solvers {
bool areEdgesOriented(const Cube& cube);

Algorithm solveEdgeOrientation(const Cube& cube);
}  // namespace solvers
