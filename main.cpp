#include "Algorithm.h"
#include "Blindsolving.h"
#include "tests/blindsolving/TestBlindsolvingMoveHash.h"
#include "tests/blindsolving/TestCornerCycleSequenceIterator.h"
#include "tests/blindsolving/TestEdgeCycleSequenceIterator.h"
#include "tests/blindsolving/TestReconstructionIterator.h"
#include "tests/core/TestPLLs.h"
#include "tests/solvers/TestEdgeOrientationSolver.h"
#include <fstream>
#include <iostream>
#include <sstream>

static std::pair<Algorithm, Algorithm> loadInput(const std::string& file_name) {
  std::ifstream file(file_name);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return Algorithm::parseScrambleSolve(buffer.str());
}

template <size_t count = 1>
static void runTests() {
  testPLLs();
  testBlindsolvingMoveHash();
  testEdgeCycleSequenceIterator();
  testCornerCycleSequenceIterator();
  testEdgeOrientationSolver();

  for (size_t i = 0; i < count; ++i) {
    Cube cube{};
    cube.scramble();
    testReconstructionIterator(cube);
  }

  std::cout << "Passed all tests!\n";
}

static void viewReconstruction(const std::string& file_name) {
  using namespace blindsolving;
  const auto [scramble, solve] = loadInput(file_name);
  const Reconstruction reconstruction = parseSolveAttempt(solve);
  std::cout << "Attempt reconstruction:\n" << toStr(reconstruction) << '\n';
  ReconstructionIterator it = getReconstructionIterator(Cube{scramble});

  std::cout << "Processing " << it.getPeriod() << " reconstructions...\n";
  const size_t n = std::min(it.getPeriod(), 3ull);
  const std::vector<std::pair<BlindsolvingReconstruction, size_t>>
      best_reconstructions = getBestReconstructions(reconstruction, it, n);

  std::cout << "Closest reconstruction matches:\n";
  for (const auto& [recon, edit_distance] : best_reconstructions)
    std::cout << "Edit distance: " << edit_distance << ", " << recon.toStr()
              << '\n';
}

int main() {
  runTests<10>();
  viewReconstruction("tests/blindsolve3.txt");
  return 0;
}
