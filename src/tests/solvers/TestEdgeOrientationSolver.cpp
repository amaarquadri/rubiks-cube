#include "TestEdgeOrientationSolver.h"
#include "Algorithm.h"
#include "Cube.h"
#include "EdgeOrientationSolver.h"
#include <iostream>
#include <stdexcept>

using namespace solvers;

static void testMaintainEdgeOrientation() {
  static constexpr size_t Count = 1000;

  Cube cube{};
  for (size_t i = 0; i < Count; ++i) {
    cube.apply(utility::pickRandom(EdgeOrientationPreservingTurns));
    if (!areEdgesOriented(cube))
      throw std::logic_error("Edge orientation was unduly broken!");
  }
}

static void testSolveEdgeOrientation() {
  static constexpr size_t Count = 1000;

  for (size_t i = 0; i < Count; ++i) {
    Cube cube{};
    cube.scramble();
    const Algorithm solve = solveEdgeOrientation(cube);
    cube.apply(solve);
    if (!areEdgesOriented(cube))
      throw std::logic_error("Edge orientation was not solved!");
  }
}

void testEdgeOrientationSolver() {
  runEdgeOrientationSolverTests();
  testMaintainEdgeOrientation();
  testSolveEdgeOrientation();

  std::cout << "Passed all tests for EdgeOrientationSolver!\n";
}
