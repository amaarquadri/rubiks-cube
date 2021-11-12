#include "Algorithm.h"
#include "Blindsolving.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace blindsolving;

std::pair<Algorithm, Algorithm> loadInput() {
    std::ifstream file("tests/blindsolve3.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return Algorithm::parseScrambleSolve(buffer.str());
}

int main() {
    auto [scramble, solve] = loadInput();
    Reconstruction reconstruction = parseSolveAttempt(solve);
    std::cout << reconstruction.toStr() << std::endl;
    std::vector<Reconstruction> possible_reconstructions = getPossibleReconstructions(scramble);
    std::vector<unsigned int> edit_distances = sortBestReconstructions(reconstruction, possible_reconstructions);

    std::cout << "Possible Reconstructions:" << std::endl;
    for (size_t i = 0; i < possible_reconstructions.size(); i++) {
        std::cout << "Edit Distance: " << edit_distances[i] << ", " << possible_reconstructions[i].toStr() << std::endl;
    }
    return 0;
}
