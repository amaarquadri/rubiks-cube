#include "Algorithm.h"
#include "Blindsolving.h"
#include <fstream>
#include <iostream>
#include <sstream>

static std::pair<Algorithm, Algorithm> loadInput(const std::string& file_name) {
  std::ifstream file(file_name);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return Algorithm::parseScrambleSolve(buffer.str());
}

static void viewReconstruction(const std::string& file_name) {
  using namespace blindsolving;
  const auto [scramble, solve] = loadInput(file_name);
  const Reconstruction reconstruction = parseSolveAttempt(solve);
  std::cout << "Attempt reconstruction:\n" << toStr(reconstruction) << '\n';
  ReconstructionIterator it = getReconstructionIterator(Cube{scramble});

  std::cout << "Processing " << it.getPeriod() << " reconstructions...\n";
  const size_t n = std::min(it.getPeriod(), 3ull);
  const std::vector<std::pair<BlindsolvingReconstruction, unsigned int>>
      best_reconstructions = getBestReconstructions(reconstruction, it, n);

  std::cout << "Closest reconstruction matches:\n";
  for (const auto& [recon, edit_distance] : best_reconstructions)
    std::cout << "Edit distance: " << edit_distance << ", " << recon.toStr()
              << '\n';
}

int main() {
  viewReconstruction("tests/blindsolve3.txt");
  return 0;
}
