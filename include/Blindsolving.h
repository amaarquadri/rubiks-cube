#pragma once

#include "Algorithm.h"
#include "EdgeLocation.h"
#include "CornerLocation.h"
#include <map>
#include <utility>

class Blindsolving {
    static const std::map<EdgeLocation, char> EDGE_LETTERING;
    static const std::map<CornerLocation, char> CORNER_LETTERING;
    static const std::map<char, Algorithm> EDGE_ALGS;
    static const std::map<char, Algorithm> CORNER_ALGS;
    static const Algorithm PARITY_ALG;

    struct SolveData {
        bool is_parsed;
        union {
            char alg;
            Algorithm unknown_moves;
        };

        explicit SolveData(const char &alg) : is_parsed(true), alg(alg) {}

        explicit SolveData(Algorithm unknown_moves) : is_parsed(false), unknown_moves(std::move(unknown_moves)) {}

        SolveData& operator=(const SolveData &other);

        ~SolveData() {};
    };

    static std::vector<SolveData> parseSolveAttempt(const Algorithm &scramble, Algorithm moves);
};
