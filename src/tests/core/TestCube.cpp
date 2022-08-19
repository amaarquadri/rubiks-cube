#include "TestCube.h"
#include "Algorithm.h"
#include "Cube.h"
#include <iostream>

void testCube() {
  const Cube checkerboard{Algorithm::parse("M2 E2 S2")};
  std::cout << "Should be a checkerboard:\n"
            << checkerboard.toNetString() << '\n';
}
