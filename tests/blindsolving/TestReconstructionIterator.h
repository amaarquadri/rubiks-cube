#pragma once

#include "Algorithm.h"
#include "Blindsolving.h"
#include "Cube.h"
#include "ReconstructionIterator.h"
#include <iostream>
#include <stdexcept>

void testReconstructionIterator(const Algorithm& scramble) {
  blindsolving::ReconstructionIterator it =
      blindsolving::getReconstructionIterator(scramble);
  std::cout << "Testing " << it.getPeriod() << " reconstructions...\n";
  do {
    Cube cube{};
    cube.apply(scramble);
    (*it).applyTo(cube);
    if (!cube.isSolved())
      throw std::logic_error(
          "BlindsolvingReconstruction did not solve the scramble!");
  } while (++it);
  std::cout << "Passed all tests for ReconstructionIterator!\n";
}
