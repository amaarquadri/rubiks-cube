#pragma once

#include "Blindsolving.h"
#include "CornerCycleSequenceIterator.h"
#include "CornerRotationAmount.h"
#include "Cube.h"
#include "Utils.h"
#include <iostream>
#include <vector>

static Cube applyCornerAlgs(const std::vector<char>& chars) {
  Cube cube{};
  for (const char& c : chars)
    cube.cycleCorners<2>({blindsolving::CORNER_BUFFER,
                          blindsolving::REVERSE_CORNER_LETTERING.at(c)});
  return cube;
}

void testCornerCycleSequenceIterator() {
  std::vector<std::vector<char>> cycles;
  cycles.push_back({'B', 'C'});
  cycles.push_back({'D', 'G', 'U'});
  cycles.push_back({'Z', 'K'});
  std::vector<CornerRotationAmount> rotation_amounts{
      CornerRotationAmount::NONE, CornerRotationAmount::CLOCKWISE,
      CornerRotationAmount::COUNTERCLOCKWISE};
  blindsolving::CornerCycleSequenceIterator it{cycles, rotation_amounts};
  const Cube cube = applyCornerAlgs(*it);
  while (++it)
    if (cube != applyCornerAlgs(*it)) throw std::logic_error("Mismatch!");

  std::cout << "Passed all tests for CornerCycleSequenceIterator!\n";
}
