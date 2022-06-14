#include "TestEdgeOrientationSolver.h"
#include "EdgeOrientationSolver.h"
#include <iostream>

void testEdgeOrientationSolver() {
  solvers::runEdgeOrientationSolverTests();
  std::cout << "Passed all tests for EdgeOrientationSolver!\n";
}
