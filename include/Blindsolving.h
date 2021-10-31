#pragma once

#include "Algorithm.h"
#include "PLL.h"
#include <map>

namespace blindsolving {
    static const Algorithm L = Algorithm::parse("R U' R' U' R U R' F' R U R' U' R' F R"); // NOLINT(cert-err58-cpp)
    const std::map<char, Algorithm> CORNERS = { // NOLINT(cert-err58-cpp)
            {'B', Jb_PERM.withSetup("U'")},
            {'C', Y_PERM},
            {'D', Ja_PERM.withSetup("U2")},
            {'E', L.withSetup("F' D")},
            {'F', L.withSetup("R U'")},
            {'G', Y_PERM.withSetup("R")},
            {'H', L.withSetup("D")},
            {'I', L.withSetup("R'")},
            {'J', L.withSetup("R2")},
            {'K', L.withSetup("R")},
            {'L', L},
            {'M', Y_PERM.withSetup("R'")},
            {'O', L.withSetup("D' R")},
            {'P', L.withSetup("D'")},
            {'R', Y_PERM.withSetup("F")},
            {'S', Y_PERM.withSetup("D R")},
            {'T', L.withSetup("D2")},
            {'U', L.withSetup("F'")},
            {'V', Y_PERM.withSetup("D R2")},
            {'W', Y_PERM.withSetup("R2")},
            {'Z', Y_PERM.withSetup("D' R2")}};

    static const Algorithm A = Algorithm::parse("M2"); // NOLINT(cert-err58-cpp)
    static const Algorithm C = Algorithm::parse("U2 M' U2 M'"); // NOLINT(cert-err58-cpp)
    static const Algorithm E = Algorithm::parse("D M' U R2 U' M U R2 U' D' M2"); // NOLINT(cert-err58-cpp)
    const std::map<char, Algorithm> EDGES = { // NOLINT(cert-err58-cpp)
            {'A', A},
            {'B', A.withSetup("R U R' U'")},
            {'C', C},
            {'D', A.withSetup("L' U' L U")},
            {'E', E},
            {'F', A.withSetup("U R U'")},
            {'H', A.withSetup("U' L' U")},
            {'I', A.withSetup("B' R B")},
            {'J', A.withSetup("R B' R' B")},
            {'K', A.withSetup("B' R' B")},
            {'L', A.withSetup("B' R2 B")},
            {'M', Algorithm::parse("U' M'") * 3 + Algorithm::parse("U' M") + Algorithm::parse("U' M'") * 4},
            {'N', A.withSetup("U' L U")},
            {'O', E.inv()},
            {'P', A.withSetup("U R' U'")},
            {'Q', A.withSetup("B L' B'")},
            {'R', A.withSetup("B L2 B'")},
            {'S', A.withSetup("B L B'")},
            {'T', A.withSetup("L' B L B'")},
            {'V', A.withSetup("U R2 U'")},
            {'W', C.inv()},
            {'Z', A.withSetup("U' L2 U")},
    };

    const Algorithm PARITY = A.withSetup("D' L2 D"); // NOLINT(cert-err58-cpp)
}
