// #define NDEBUG // uncomment to disable asserts

#include "Algorithm.h"
#include "Blindsolving.h"
#include "RunTests.h"
#include "SolveAttemptParsingUtils.h"
#include <iostream>

static void viewReconstruction(const std::string& file_name) {
  using namespace blindsolving;
  const auto [scramble_str, solve_str] = utility::loadScrambleSolve(file_name);
  const Algorithm scramble = Algorithm::parse(scramble_str);
  // use parseExpanded for the attempted solution so that the division between
  // blindsolving moves can easily be determined
  const Algorithm solve = Algorithm::parseExpanded(solve_str);

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
  runTests();
  viewReconstruction("tests/blindsolve3.txt");
  return 0;
}
