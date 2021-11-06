#pragma once

#include "Algorithm.h"

namespace blindsolving {
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
}
