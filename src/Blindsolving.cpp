#include "Blindsolving.h"
#include "Cube.h"
#include "PLL.h"

static std::map<EdgeLocation, char> getEdgeLettering() {
    std::map<EdgeLocation, char> edge_lettering;
    std::string edge_labels = "ABCDFJNRUVWZ";
    std::string flipped_edge_labels = "MIEQLPTHGKOS";
    for (size_t i = 0; i < Cube::EDGE_LOCATION_ORDER.size(); i++) {
        EdgeLocation location = Cube::EDGE_LOCATION_ORDER[i];
        edge_lettering[location] = edge_labels[i];
        edge_lettering[location.flip()] = flipped_edge_labels[i];
    }
    return edge_lettering;
}

static std::map<CornerLocation, char> getCornerLettering() {
    std::map<CornerLocation, char> corner_lettering;
    std::string corner_labels = "ABCDUVWZ";
    std::string clockwise_corner_labels = "QMIESGKO";
    std::string counterclockwise_corner_labels = "NJFRHLPT";
    for (size_t i = 0; i < Cube::CORNER_LOCATION_ORDER.size(); i++) {
        CornerLocation location = Cube::CORNER_LOCATION_ORDER[i];
        corner_lettering[location] = corner_labels[i];
        corner_lettering[location.rotateClockwise()] = clockwise_corner_labels[i];
        corner_lettering[location.rotateCounterClockwise()] = counterclockwise_corner_labels[i];
    }
    return corner_lettering;
}

const std::map<EdgeLocation, char> Blindsolving::EDGE_LETTERING = getEdgeLettering(); // NOLINT(cert-err58-cpp)
const std::map<CornerLocation, char> Blindsolving::CORNER_LETTERING = getCornerLettering(); // NOLINT(cert-err58-cpp)

static const Algorithm A_ALG = Algorithm::parse("M2"); // NOLINT(cert-err58-cpp)
static const Algorithm C_ALG = Algorithm::parse("U2 M' U2 M'"); // NOLINT(cert-err58-cpp)
static const Algorithm E_ALG = Algorithm::parse("D M' U R2 U' M U R2 U' D' M2"); // NOLINT(cert-err58-cpp)
const std::map<char, Algorithm> Blindsolving::EDGE_ALGS = { // NOLINT(cert-err58-cpp)
        {'A', A_ALG},
        {'B', A_ALG.withSetup("R U R' U'")},
        {'C', C_ALG},
        {'D', A_ALG.withSetup("L' U' L U")},
        {'E', E_ALG},
        {'F', A_ALG.withSetup("U R U'")},
        {'H', A_ALG.withSetup("U' L' U")},
        {'I', A_ALG.withSetup("B' R B")},
        {'J', A_ALG.withSetup("R B' R' B")},
        {'K', A_ALG.withSetup("B' R' B")},
        {'L', A_ALG.withSetup("B' R2 B")},
        {'M', Algorithm::parse("U' M'") * 3 + Algorithm::parse("U' M") + Algorithm::parse("U' M'") * 4},
        {'N', A_ALG.withSetup("U' L U")},
        {'O', E_ALG.inv()},
        {'P', A_ALG.withSetup("U R' U'")},
        {'Q', A_ALG.withSetup("B L' B'")},
        {'R', A_ALG.withSetup("B L2 B'")},
        {'S', A_ALG.withSetup("B L B'")},
        {'T', A_ALG.withSetup("L' B L B'")},
        {'V', A_ALG.withSetup("U R2 U'")},
        {'W', C_ALG.inv()},
        {'Z', A_ALG.withSetup("U' L2 U")},
};

static const Algorithm L_ALG = Algorithm::parse("R U' R' U' R U R' F' R U R' U' R' F R"); // NOLINT(cert-err58-cpp)
const std::map<char, Algorithm> Blindsolving::CORNER_ALGS = { // NOLINT(cert-err58-cpp)
        {'B', Jb_PERM.withSetup("U'")},
        {'C', Y_PERM},
        {'D', Ja_PERM.withSetup("U2")},
        {'E', L_ALG.withSetup("F' D")},
        {'F', L_ALG.withSetup("R U'")},
        {'G', Y_PERM.withSetup("R")},
        {'H', L_ALG.withSetup("D")},
        {'I', L_ALG.withSetup("R'")},
        {'J', L_ALG.withSetup("R2")},
        {'K', L_ALG.withSetup("R")},
        {'L', L_ALG},
        {'M', Y_PERM.withSetup("R'")},
        {'O', L_ALG.withSetup("D' R")},
        {'P', L_ALG.withSetup("D'")},
        {'R', Y_PERM.withSetup("F")},
        {'S', Y_PERM.withSetup("D R")},
        {'T', L_ALG.withSetup("D2")},
        {'U', L_ALG.withSetup("F'")},
        {'V', Y_PERM.withSetup("D R2")},
        {'W', Y_PERM.withSetup("R2")},
        {'Z', Y_PERM.withSetup("D' R2")}};

const Algorithm Blindsolving::PARITY_ALG = A_ALG.withSetup("D' L2 D"); // NOLINT(cert-err58-cpp)

std::vector<Blindsolving::SolveData> Blindsolving::parseSolveAttempt(const Algorithm &scramble, Algorithm moves) {
    std::vector<Blindsolving::SolveData> solve;
    Cube cube{};
    cube.apply(scramble);

    while (moves.length() > 0) {
        for (int i = 0; i < moves.length(); i++) {
            Algorithm some_moves = mo
        }
    }
    return solve;
}

Blindsolving::SolveData& Blindsolving::SolveData::operator=(const Blindsolving::SolveData &other) {
    is_parsed = other.is_parsed;
    if (is_parsed) alg = other.alg;
    else unknown_moves = other.unknown_moves;
    return *this;
}
