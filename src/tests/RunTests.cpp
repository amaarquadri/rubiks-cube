#include "RunTests.h"
#include "TestAlgorithm.h"
#include "TestBlindsolvingMoveHash.h"
#include "TestCombination.h"
#include "TestCornerCycleSequenceIterator.h"
#include "TestCubeOrientation.h"
#include "TestDominoReductionSolver.h"
#include "TestEdgeCycleSequenceIterator.h"
#include "TestEdgeOrientationSolver.h"
#include "TestPLLs.h"
#include "TestPackedBitsArray.h"
#include "TestPermutation.h"
#include "TestReconstructionIterator.h"
#include <cstddef>
#include <iostream>

void runTests() {
  testCombination();
  testPermutation();
  testPackedBitsArray();
  testAlgorithm();
  testSolveCubeOrientation();
  testPLLs();
  testBlindsolvingMoveHash();
  testCornerCycleSequenceIterator();
  testEdgeCycleSequenceIterator();
  testEdgeOrientationSolver();
  //  testDominoReductionSolver();

  static constexpr size_t Count = 1;
  for (size_t i = 0; i < Count; ++i) {
    Cube cube{};
    cube.scramble();
    testReconstructionIterator(cube);
  }

  std::cout << "Passed all tests!\n";
}
