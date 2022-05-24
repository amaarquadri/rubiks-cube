#pragma once

#include "Algorithm.h"
#include "BlindsolvingReconstruction.h"
#include "CornerLocation.h"
#include "Cube.h"
#include "EdgeLocation.h"
#include "Reconstruction.h"
#include "ReconstructionIterator.h"
#include <unordered_map>

namespace blindsolving {
extern const std::unordered_map<char, Algorithm> EDGE_ALGS;
extern const std::unordered_map<char, Algorithm> CORNER_ALGS;
extern const Algorithm PARITY_ALG;
constexpr EdgeLocation EDGE_BUFFER{Face::D, Face::F};
constexpr CornerLocation CORNER_BUFFER{Face::U, Face::L, Face::B};

Reconstruction parseSolveAttempt(const Algorithm& moves);

/**
 * @param cube The Cube to create a ReconstructionIterator for. It is passed by
 * value so that a local copy is created which can be modified in the function.
 */
ReconstructionIterator getReconstructionIterator(Cube cube);

/**
 * @param n The maximum number of BlindsolvingReconstructions to include in the
 * result.
 */
std::vector<std::pair<BlindsolvingReconstruction, unsigned int>>
getBestReconstructions(const Reconstruction& solve, ReconstructionIterator& it,
                       const size_t& max);
}  // namespace blindsolving
