#include "TestThistlethwaiteSolver.h"
#include "Algorithm.h"
#include "Cube.h"
#include "ThistlethwaiteSolver.h"
#include <iostream>
#include <stdexcept>

using namespace solvers;

static void testSolveThistlethwaite() {
  static constexpr size_t Count = 1000;

  for (size_t i = 0; i < Count; ++i) {
    Cube cube{};
    cube.scramble();
    const Algorithm solve = solveThistlethwaite(cube);
    cube.apply(solve);
    if (!cube.isSolved()) throw std::logic_error("Cube was not solved!");
  }
}

void testThistlethwaiteSolver() {
  runThistlethwaiteSolverTests();
  testSolveThistlethwaite();

  std::cout << "Passed all tests for ThistlethwaiteSolver!\n";
}
