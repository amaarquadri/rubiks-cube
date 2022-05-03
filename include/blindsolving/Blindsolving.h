#pragma once

#include "Algorithm.h"
#include "CornerLocation.h"
#include "Cube.h"
#include "EdgeLocation.h"
#include "Reconstruction.h"
#include <map>
#include <unordered_map>

namespace blindsolving {
extern const std::map<char, Algorithm> EDGE_ALGS;
extern const std::map<char, Algorithm> CORNER_ALGS;
extern const Algorithm PARITY_ALG;

Reconstruction parseSolveAttempt(const Algorithm& moves);

std::vector<Reconstruction> getPossibleReconstructions(
    const Algorithm& scramble);

std::vector<unsigned int> sortBestReconstructions(
    const Reconstruction& solve,
    std::vector<Reconstruction>& possible_reconstructions);
}  // namespace blindsolving
