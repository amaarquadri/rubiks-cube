#pragma once

#include "Blindsolving.h"
#include "Cube.h"
#include "EdgeCycleSequenceIterator.h"
#include "Utils.h"
#include <iostream>
#include <vector>

static Cube applyEdgeAlgs(const std::vector<char>& chars) {
  Cube cube{};
  for (const char& c : chars)
    cube.cycleEdges<2>({blindsolving::EDGE_BUFFER,
                        blindsolving::REVERSE_EDGE_LETTERING.at(c)});
  return cube;
}

static void testEdgeCycleSequenceIterator() {
  std::vector<std::vector<char>> cycles;
  cycles.push_back({'A', 'B', 'C'});
  cycles.push_back({'D', 'F'});
  cycles.push_back({'H', 'J', 'K', 'N'});
  std::vector<bool> are_even{false, true, false};
  blindsolving::EdgeCycleSequenceIterator it{cycles, are_even};
  const Cube cube = applyEdgeAlgs(*it);
  while (++it)
    if (cube != applyEdgeAlgs(*it)) throw std::logic_error("Mismatch!");

  std::cout << "Passed all tests for EdgeCycleSequenceIterator!\n";
}
