#pragma once

#include "Algorithm.h"
#include "EdgeLocation.h"
#include "CornerLocation.h"
#include <unordered_map>
#include <map>
#include <utility>

class Blindsolving {
    static const std::unordered_map<EdgeLocation, char> EDGE_LETTERING;
    static const std::unordered_map<CornerLocation, char> CORNER_LETTERING;
    static const std::map<char, Algorithm> EDGE_ALGS;
    static const std::map<char, Algorithm> CORNER_ALGS;
    static const Algorithm PARITY_ALG;

public:
    struct SolveData {
        bool is_parsed;
        union {
            struct { // only valid if is_parsed
                bool is_parity;
                bool is_edge; // only valid if !is_parity
                char alg; // only valid if !is_parity
            };
            Algorithm unknown_moves; // only valid if !is_parsed
        };

        SolveData() : is_parsed(true), is_parity(true) {}

        explicit SolveData(const bool &is_edge, const char &alg) :
                is_parsed(true), is_parity(false), is_edge(is_edge), alg(alg) {}

        explicit SolveData(Algorithm unknown_moves) :
                is_parsed(false), unknown_moves(std::move(unknown_moves)) {}

        explicit SolveData(const SolveData &other);

        SolveData& operator=(const SolveData &other);

        ~SolveData() {};
    };

    static std::vector<SolveData> parseSolveAttempt(const Algorithm &moves);
};
