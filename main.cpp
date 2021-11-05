#include "Algorithm.h"
#include "Blindsolving.h"
#include "Cube.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::pair<Algorithm, Algorithm> loadInput() {
    std::ifstream file("tests/blindsolve1.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return Algorithm::parseScrambleSolve(buffer.str());
}

int main() {
    auto [scramble, solve] = loadInput();
    std::vector<Blindsolving::SolveData> solve_data = Blindsolving::parseSolveAttempt(solve);
    Cube cube{};
    cube.apply(moves);
    std::cout << cube.isSolved() << std::endl;
    std::cout << cube.toNetString() << std::endl;
    return 0;
}
