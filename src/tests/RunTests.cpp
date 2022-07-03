#include "RunTests.h"
#include "TestAlgorithm.h"
#include "TestBlindsolvingMoveHash.h"
#include "TestCombination.h"
#include "TestCornerCycleSequenceIterator.h"
#include "TestCubeOrientation.h"
#include "TestEdgeCycleSequenceIterator.h"
#include "TestEdgeOrientationSolver.h"
#include "TestPLLs.h"
#include "TestPermutation.h"
#include "TestReconstructionIterator.h"
#include "TestUint24Array.h"
#include <cstddef>
#include <iostream>

void runTests() {
  testCombination();
  testPermutation();
  testUint24Array();
  testAlgorithm();
  testSolveCubeOrientation();
  testPLLs();
  testBlindsolvingMoveHash();
  testCornerCycleSequenceIterator();
  testEdgeCycleSequenceIterator();
  testEdgeOrientationSolver();

  static constexpr size_t Count = 10;
  for (size_t i = 0; i < Count; ++i) {
    Cube cube{};
    cube.scramble();
    testReconstructionIterator(cube);
  }

  std::cout << "Passed all tests!\n";
}
