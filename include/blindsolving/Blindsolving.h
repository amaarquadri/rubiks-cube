#pragma once

#include "Algorithm.h"
#include "BlindsolvingReconstruction.h"
#include "CornerLocation.h"
#include "Cube.h"
#include "EdgeLocation.h"
#include "Reconstruction.h"
#include "ReconstructionIterator.h"
#include <map>
#include <unordered_map>

namespace blindsolving {
extern const std::map<char, Algorithm> EDGE_ALGS;
extern const std::map<char, Algorithm> CORNER_ALGS;
extern const Algorithm PARITY_ALG;

Reconstruction parseSolveAttempt(const Algorithm& moves);

ReconstructionIterator getReconstructionIterator(const Algorithm& scramble);

/**
 * @param n The maximum number of BlindsolvingReconstructions to include in the
 * result.
 */
std::vector<std::pair<BlindsolvingReconstruction, unsigned int>>
getBestReconstructions(const Reconstruction& solve, ReconstructionIterator& it,
                       const size_t& max);
}  // namespace blindsolving
