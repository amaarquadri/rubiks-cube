#pragma once

#include "Algorithm.h"
#include "Blindsolving.h"
#include "Cube.h"
#include "ReconstructionIterator.h"
#include <iostream>
#include <stdexcept>
#include <unordered_set>

void testReconstructionIterator(const Algorithm& scramble) {
  const Cube scrambled_cube = [&]() {
    Cube cube{};
    cube.apply(scramble);
    return cube;
  }();

  blindsolving::ReconstructionIterator it =
      blindsolving::getReconstructionIterator(scramble);
  std::unordered_set<blindsolving::BlindsolvingReconstruction> recons{};
  std::cout << "Testing " << it.getPeriod() << " reconstructions...\n";
  size_t i = 0;  // only used if info == true
  do {
    const blindsolving::BlindsolvingReconstruction recon = *it;
    if (!recons.insert(recon).second)
      throw std::logic_error("Duplicate reconstruction!");
    Cube cube = scrambled_cube;
    recon.applyTo(cube);
    if (!cube.isSolved())
      throw std::logic_error(
          "BlindsolvingReconstruction did not solve the scramble!");
    ++i;
    if (i % 5000 == 0) std::cout << "Passed " << i << " reconstructions...\n";
  } while (++it);
  if (recons.size() != it.getPeriod())
    throw std::logic_error("Incorrect number of unique reconstructions!");
  std::cout << "Passed all tests for ReconstructionIterator!\n";
}
