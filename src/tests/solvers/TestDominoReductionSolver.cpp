#include "TestDominoReductionSolver.h"
#include "Algorithm.h"
#include "Cube.h"
#include "DominoReductionSolver.h"
#include <iostream>
#include <stdexcept>

using namespace solvers;

static void testMaintainDominoReduction() {
  static constexpr size_t Count = 1000;

  Cube cube{};
  for (size_t i = 0; i < Count; ++i) {
    cube.apply(utility::pickRandom(DominoReductionPreservingTurns));
    if (!isDominoReduced(cube))
      throw std::logic_error("Domino reduction was unduly broken!");
  }
}

static void testSolveDominoReduction() {
  static constexpr size_t Count = 1000;

  for (size_t i = 0; i < Count; ++i) {
    Cube cube{};
    cube.scramble();
    const Algorithm solve = solveDominoReduction(cube);
    cube.apply(solve);
    if (!isDominoReduced(cube))
      throw std::logic_error("Domino reduction was not achieved!");
  }
}

void testDominoReductionSolver() {
  solvers::runDominoReductionSolverTests();
  testMaintainDominoReduction();
  testSolveDominoReduction();

  std::cout << "Passed all tests for DominoReductionSolver!\n";
}
