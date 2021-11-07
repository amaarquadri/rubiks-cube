#pragma once

#include "Algorithm.h"
#include "EdgeLocation.h"
#include "CornerLocation.h"
#include "Reconstruction.h"
#include "Cube.h"
#include <unordered_map>
#include <map>

namespace blindsolving {
    extern const std::unordered_map<EdgeLocation, char> EDGE_LETTERING;
    extern const std::unordered_map<CornerLocation, char> CORNER_LETTERING;
    extern const std::map<char, Algorithm> EDGE_ALGS;
    extern const std::map<char, Algorithm> CORNER_ALGS;
    extern const Algorithm PARITY_ALG;

    Reconstruction parseSolveAttempt(const Algorithm &moves);

    std::vector<Reconstruction> getPossibleReconstructions(const Algorithm &scramble);

    std::vector<unsigned int> sortBestReconstructions(const Reconstruction &solve, std::vector<Reconstruction> &possible_reconstructions);
}
