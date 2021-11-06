#include "Algorithm.h"
#include "Blindsolving.h"
#include "Cube.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::pair<Algorithm, Algorithm> loadInput() {
    std::ifstream file("tests/blindsolve2.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return Algorithm::parseScrambleSolve(buffer.str());
}

int main() {
    auto [scramble, solve] = loadInput();
    blindsolving::Reconstruction reconstruction = blindsolving::parseSolveAttempt(solve);
    for (blindsolving::SolveData &i: reconstruction.solve_data) std::cout << i.toStr() << std::endl;

//    Cube cube{};
//    cube.apply(Algorithm::parse("U R U' M2 U R' U'"));
//    std::cout << cube.isSolved() << std::endl;
//    std::cout << cube.toNetString() << std::endl;
    return 0;
}
