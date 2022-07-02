#include "TestCubeOrientation.h"
#include "CubeOrientation.h"
#include "Face.h"
#include <array>
#include <iostream>
#include <stdexcept>

void testSolveCubeOrientation() {
  using enum Face;
  static constexpr std::array<Face, 6> Faces{U, F, R, B, L, D};
  for (const Face& top : Faces)
    for (const Face& front : Faces) {
      CubeOrientation orientation{top, front};
      if (!orientation.isValid()) continue;
      const auto [first, second] = orientation.solve();
      if (first) {
        orientation *= first.value();
        if (second) orientation *= second.value();
      }
      if (orientation != CubeOrientation::identity())
        throw std::logic_error("CubeOrientation was not solved!");
    }
  std::cout << "Passed all tests for CubeOrientation!\n";
}
