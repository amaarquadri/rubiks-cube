#include "TestAlgorithm.h"
#include "Algorithm.h"
#include "Cube.h"
#include "SolveAttemptParsingUtils.h"
#include <iostream>
#include <stdexcept>

void testAlgorithm() {
  for (const std::string file_name :
       {"tests/blindsolve1.txt", "tests/blindsolve2.txt",
        "tests/blindsolve3.txt"}) {
    const auto [scramble_str, solve_str] =
        utility::loadScrambleSolve(file_name);
    const Algorithm original = Algorithm::parse(solve_str);
    const Algorithm simplified = [&]() {
      Algorithm alg = original;
      alg.toStandardForm();
      return alg;
    }();
    const Algorithm combined = [&]() {
      Algorithm alg = original + simplified.inv();
      alg.cancelMoves();
      return alg;
    }();

    if (!simplified.isStandardForm())
      throw std::logic_error("Did not convert to standard form!");
    if (original != simplified)
      throw std::logic_error("Algorithm simplified incorrectly!");
    if (!combined.empty())
      throw std::logic_error("Not all moves were cancelled!");
  }
  std::cout << "Passed all tests for Algorithm!\n";
}
