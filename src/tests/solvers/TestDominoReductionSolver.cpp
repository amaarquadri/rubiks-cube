#include "TestDominoReductionSolver.h"
#include "DominoReductionSolver.h"
#include <iostream>

void testDominoReductionSolver() {
  solvers::runDominoReductionSolverTests();
  std::cout << "Passed all tests for DominoReductionSolver!\n";
}
