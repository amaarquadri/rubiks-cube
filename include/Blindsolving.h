#pragma once

#include "Algorithm.h"
#include "PLL.h"

namespace blindsolving {
    namespace corners {
        const Algorithm L = Algorithm::parse("R U' R' U' R U R' F' R U R' U' R' F R"); // NOLINT(cert-err58-cpp)
        const Algorithm B = Jb_PERM.withSetup("U'"); // NOLINT(cert-err58-cpp)
        const Algorithm C = Y_PERM; // NOLINT(cert-err58-cpp)
        const Algorithm D = Ja_PERM.withSetup("U2"); // NOLINT(cert-err58-cpp)
        const Algorithm E = L.withSetup("F' D"); // NOLINT(cert-err58-cpp)
        const Algorithm F = L.withSetup("R U'"); // NOLINT(cert-err58-cpp)
        const Algorithm G = Y_PERM.withSetup("R"); // NOLINT(cert-err58-cpp)
        const Algorithm H = L.withSetup("D"); // NOLINT(cert-err58-cpp)
        const Algorithm I = L.withSetup("R'"); // NOLINT(cert-err58-cpp)
        const Algorithm J = L.withSetup("R2"); // NOLINT(cert-err58-cpp)
        const Algorithm K = L.withSetup("R"); // NOLINT(cert-err58-cpp)
        const Algorithm M = Y_PERM.withSetup("R'"); // NOLINT(cert-err58-cpp)
        const Algorithm O = L.withSetup("D' R"); // NOLINT(cert-err58-cpp)
        const Algorithm P = L.withSetup("D'"); // NOLINT(cert-err58-cpp)
        const Algorithm R = Y_PERM.withSetup("F"); // NOLINT(cert-err58-cpp)
        const Algorithm S = Y_PERM.withSetup("D R"); // NOLINT(cert-err58-cpp)
        const Algorithm T = L.withSetup("D2"); // NOLINT(cert-err58-cpp)
        const Algorithm U = L.withSetup("F'"); // NOLINT(cert-err58-cpp)
        const Algorithm V = Y_PERM.withSetup("D R2"); // NOLINT(cert-err58-cpp)
        const Algorithm W = Y_PERM.withSetup("R2"); // NOLINT(cert-err58-cpp)
        const Algorithm Z = Y_PERM.withSetup("D' R2"); // NOLINT(cert-err58-cpp)
    }

    namespace edges {
        const Algorithm A = Algorithm::parse("M2"); // NOLINT(cert-err58-cpp)
        const Algorithm B = A.withSetup("R U R' U'"); // NOLINT(cert-err58-cpp)
        const Algorithm C = Algorithm::parse("U2 M' U2 M'"); // NOLINT(cert-err58-cpp)
        const Algorithm D = A.withSetup("L' U' L U"); // NOLINT(cert-err58-cpp)
        const Algorithm E = Algorithm::parse("D M' U R2 U' M U R2 U' D' M2"); // NOLINT(cert-err58-cpp)
        const Algorithm F = A.withSetup("U R U'"); // NOLINT(cert-err58-cpp)
        const Algorithm H = A.withSetup("U' L' U"); // NOLINT(cert-err58-cpp)
        const Algorithm I = A.withSetup("B' R B"); // NOLINT(cert-err58-cpp)
        const Algorithm J = A.withSetup("R B' R' B"); // NOLINT(cert-err58-cpp)
        const Algorithm K = A.withSetup("B' R' B"); // NOLINT(cert-err58-cpp)
        const Algorithm L = A.withSetup("B' R2 B"); // NOLINT(cert-err58-cpp)
        const Algorithm M = Algorithm::parse("U' M'") * 3 + Algorithm::parse("U' M") + Algorithm::parse("U' M'") * 4; // NOLINT(cert-err58-cpp)
        const Algorithm N = A.withSetup("U' L U"); // NOLINT(cert-err58-cpp)
        const Algorithm O = E.inv(); // NOLINT(cert-err58-cpp)
        const Algorithm P = A.withSetup("U R' U'"); // NOLINT(cert-err58-cpp)
        const Algorithm Q = A.withSetup("B L' B'"); // NOLINT(cert-err58-cpp)
        const Algorithm R = A.withSetup("B L2 B'"); // NOLINT(cert-err58-cpp)
        const Algorithm S = A.withSetup("B L B'"); // NOLINT(cert-err58-cpp)
        const Algorithm T = A.withSetup("L' B L B'"); // NOLINT(cert-err58-cpp)
        const Algorithm V = A.withSetup("U R2 U'"); // NOLINT(cert-err58-cpp)
        const Algorithm W = C.inv(); // NOLINT(cert-err58-cpp)
        const Algorithm Z = A.withSetup("U' L2 U"); // NOLINT(cert-err58-cpp)
    }

    const Algorithm PARITY = edges::A.withSetup("D' L2 D"); // NOLINT(cert-err58-cpp)
}
