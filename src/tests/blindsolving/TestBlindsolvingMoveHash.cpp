#include "TestBlindsolvingMoveHash.h"
#include "BlindsolvingMove.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

void testBlindsolvingMoveHash() {
  using blindsolving::BlindsolvingMove;
  static constexpr std::hash<BlindsolvingMove> hasher{};
  std::unordered_set<size_t> hashes{};
  hashes.insert(hasher({}));  // parity
  for (char alg = 'A'; alg <= 'Z'; alg++) {
    if (alg == 'X' || alg == 'Y') continue;  // do not correspond to targets
    for (const bool& is_edge : {true, false}) {
      if (is_edge ? (alg == 'G' || alg == 'U')
                  : (alg == 'A' || alg == 'Q' || alg == 'N'))
        continue;  // invalid combinations
      if (!hashes.insert(hasher({is_edge, alg})).second)
        throw std::logic_error("Hash collision!");
    }
  }
  if (hashes.size() != 44)
    throw std::logic_error("Incorrect number of elements!");
  std::cout << "Passed all tests for hash<BlindsolvingMove>!\n";
}
