#include "TestHalfTurnReductionSolver.h"
#include "Algorithm.h"
#include "Cube.h"
#include "HalfTurnReductionSolver.h"
#include <iostream>
#include <stdexcept>

using namespace solvers;

static void testMaintainHalfTurnReduction() {
  static constexpr size_t Count = 1000;

  Cube cube{};
  for (size_t i = 0; i < Count; ++i) {
    cube.apply(utility::pickRandom(HalfTurnReductionPreservingTurns));
    if (!isHalfTurnReduced(cube))
      throw std::logic_error("Half turn reduction was unduly broken!");
  }
}

static void testSolveHalfTurnReduction() {
  static constexpr size_t Count = 1000;

  for (size_t i = 0; i < Count; ++i) {
    const Algorithm scramble = Algorithm::random(20);
    Cube cube{scramble};
    const Algorithm solve = solveHalfTurnReduction(cube);
    cube.apply(solve);
    if (!isHalfTurnReduced(cube))
      throw std::logic_error("Half turn reduction was not solved!");
  }
}

void testHalfTurnReductionSolver() {
  solvers::runHalfTurnReductionSolverTests();
  testMaintainHalfTurnReduction();
  testSolveHalfTurnReduction();

  std::cout << "Passed all tests for HalfTurnReductionSolver!\n";
}
