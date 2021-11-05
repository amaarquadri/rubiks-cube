#include "Blindsolving.h"
#include "Cube.h"
#include "PLL.h"

static std::unordered_map<EdgeLocation, char> getEdgeLettering() {
    std::unordered_map<EdgeLocation, char> edge_lettering;
    std::string edge_labels = "ABCDFJNRUVWZ";
    std::string flipped_edge_labels = "MIEQLPTHGKOS";
    for (size_t i = 0; i < Cube::EDGE_LOCATION_ORDER.size(); i++) {
        EdgeLocation location = Cube::EDGE_LOCATION_ORDER[i];
        edge_lettering.insert({location, edge_labels[i]});
        edge_lettering.insert({location.flip(), flipped_edge_labels[i]});
    }
    return edge_lettering;
}

static std::unordered_map<CornerLocation, char> getCornerLettering() {
    std::unordered_map<CornerLocation, char> corner_lettering;
    std::string corner_labels = "ABCDUVWZ";
    std::string clockwise_corner_labels = "QMIESGKO";
    std::string counterclockwise_corner_labels = "NJFRHLPT";
    for (size_t i = 0; i < Cube::CORNER_LOCATION_ORDER.size(); i++) {
        CornerLocation location = Cube::CORNER_LOCATION_ORDER[i];
        corner_lettering.insert({location, corner_labels[i]});
        corner_lettering.insert({location.rotateClockwise(), clockwise_corner_labels[i]});
        corner_lettering.insert({location.rotateCounterClockwise(), counterclockwise_corner_labels[i]});
    }
    return corner_lettering;
}

const std::unordered_map<EdgeLocation, char> Blindsolving::EDGE_LETTERING = getEdgeLettering(); // NOLINT(cert-err58-cpp)
const std::unordered_map<CornerLocation, char> Blindsolving::CORNER_LETTERING = getCornerLettering(); // NOLINT(cert-err58-cpp)

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

Blindsolving::SolveData::SolveData(const Blindsolving::SolveData &other) {
    *this = other;
}

Blindsolving::SolveData& Blindsolving::SolveData::operator=(const Blindsolving::SolveData &other) {
    moves = other.moves;
    is_parsed = other.is_parsed;
    is_parity = other.is_parity;
    is_edge = other.is_edge;
    alg = other.alg;
    return *this;
}

std::string Blindsolving::SolveData::toStr() const {
    if (is_parsed) {
        if (is_parity) return "Parity";
        else return (is_edge ? "Edge: " : "Corner: ") + std::string(1, alg);
    }
    else {
        return "Unknown Moves: " + moves.toStr();
    }
}

std::vector<Blindsolving::SolveData> Blindsolving::parseSolveAttempt(const Algorithm &moves) {
    int consumed = 0;

    // extract all initial CubeRotations
    CubeOrientation orientation = CubeOrientation::identity();
    while (!moves[consumed].isTurn) {
        orientation *= moves[consumed].cubeRotation;
        consumed++;
    }
    for (size_t i = consumed + 1; i < moves.length(); i++) {
        if (!moves[i].isTurn) {
            throw std::invalid_argument("CubeRotations can only be at the start!");
        }
    }

    // set up transformations
    std::map<char, Cube> edge_alg_transformations;
    for (auto [chr, alg] : EDGE_ALGS) {
        Cube cube{};
        cube.apply(alg);
        edge_alg_transformations.insert({chr, cube});
    }
    std::map<char, Cube> corner_alg_transformations;
    for (auto [chr, alg] : CORNER_ALGS) {
        Cube cube{};
        cube.apply(alg);
        corner_alg_transformations.insert({chr, cube});
    }
    Cube parity_alg_transformation{};
    parity_alg_transformation.apply(PARITY_ALG);

    std::vector<Blindsolving::SolveData> solve;
    while (consumed < moves.length()) {
        bool found_match = false;
        SolveData solve_data{};
        Cube test_transformation{};
        for (int i = consumed; i < moves.length(); i++) {
            test_transformation.apply(orientation.apply(moves[i].turn));

            for (auto [chr, transformation] : edge_alg_transformations) {
                if (test_transformation == transformation) {
                    found_match = true;
                    solve_data.moves = moves.subAlgorithm(consumed, i + 1);
                    solve_data.is_parsed = true;
                    solve_data.is_parity = false;
                    solve_data.is_edge = true;
                    solve_data.alg = chr;
                    break;
                }
            }
            for (auto [chr, transformation] : corner_alg_transformations) {
                if (test_transformation == transformation) {
                    found_match = true;
                    solve_data.moves = moves.subAlgorithm(consumed, i + 1);
                    solve_data.is_parsed = true;
                    solve_data.is_parity = false;
                    solve_data.is_edge = false;
                    solve_data.alg = chr;
                    break;
                }
            }
            if (test_transformation == parity_alg_transformation) {
                found_match = true;
                solve_data.moves = moves.subAlgorithm(consumed, i + 1);
                // solve_data is initialized to parity anyways, so no need to update here
            }
            if (found_match) {
                solve.push_back(solve_data);
                consumed = i + 1; // we consumed [0, i] which has a length of (i + 1)
                break;
            }
        }
        if (!found_match) {
            if (solve.empty() || solve[solve.size() - 1].is_parsed) {
                solve.emplace_back(Algorithm{});
            }
            solve[solve.size() - 1].moves.push_back(moves[consumed]);
            consumed++;
        }
    }
    return solve;
}
