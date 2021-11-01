#include "Algorithm.h"
#include "Cube.h"
#include <iostream>
#include <fstream>
#include <sstream>

Algorithm loadInput() {
    std::ifstream file("input.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return Algorithm::parse(buffer.str());
}

int main() {
    Algorithm moves = loadInput();
    Cube cube{};
    cube.apply(moves);
    std::cout << cube.isSolved() << std::endl;
    return 0;
}
