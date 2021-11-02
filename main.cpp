#include "Algorithm.h"
#include "Cube.h"
#include <iostream>
#include <fstream>
#include <sstream>

Algorithm loadInput() {
    std::ifstream file("tests/no_rotations.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    Algorithm moves = Algorithm::parse(buffer.str());
    if (moves.length() == 0) {
        std::cout << "Warning no moves read from file! Check that the path is correct." << std::endl;
    }
    else {
        std::cout << "Loaded moves: " << moves.toStr() << std::endl;
    }
    return moves;
}

int main() {
    Algorithm moves = loadInput();
    Cube cube{};
    cube.apply(moves);
    std::cout << cube.isSolved() << std::endl;
    std::cout << cube.toNetString() << std::endl;
    return 0;
}
