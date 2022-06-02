#pragma once

#include <iostream>

namespace solvers {
extern void testGetEdgeOrientation();

extern void testApplyTurn();

extern void testSolveEdgeOrientation();
}  // namespace solvers

void testEdgeOrientationSolver() {
  solvers::testGetEdgeOrientation();
  solvers::testApplyTurn();
  solvers::testSolveEdgeOrientation();
  std::cout << "Passed all tests for EdgeOrientationSolver!\n";
}
