#pragma once

#include "Algorithm.h"
#include "EdgeLocation.h"
#include "CornerLocation.h"
#include "Cube.h"
#include <unordered_map>
#include <map>

namespace blindsolving {
    extern const std::unordered_map<EdgeLocation, char> EDGE_LETTERING;
    extern const std::unordered_map<CornerLocation, char> CORNER_LETTERING;
    extern const std::map<char, Algorithm> EDGE_ALGS;
    extern const std::map<char, Algorithm> CORNER_ALGS;
    extern const Algorithm PARITY_ALG;

    struct SolveData {
        Algorithm moves;
        bool is_parsed;
        bool is_parity; // only valid if is_parsed
        bool is_edge; // only valid if is_parsed && !is_parity
        char alg; // only valid if !is_parity

        SolveData() : is_parsed(true), is_parity(true) {}

        explicit SolveData(const bool &is_edge, const char &alg) :
                is_parsed(true), is_parity(false), is_edge(is_edge), alg(alg) {}

        explicit SolveData(const Algorithm &moves) : is_parsed(false), moves(moves) {}

        explicit SolveData(const SolveData &other);

        SolveData& operator=(const SolveData &other) = default;

        [[nodiscard]] bool operator==(const SolveData &other) const;

        [[nodiscard]] std::string toStr() const;

        ~SolveData() {};
    };

    std::vector<SolveData> parseSolveAttempt(const Algorithm &moves);

    bool edgesSolvedUpToParity(const Cube &cube);

    EdgeLocation getLocation(const EdgePiece &edge_piece);

    std::vector<std::vector<SolveData>> getAllBlindsolves(const Algorithm &scramble);
}
