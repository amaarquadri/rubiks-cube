#pragma once

#include "Turn.h"
#include "CubeRotation.h"
#include <vector>
#include <string>

struct Algorithm {
    struct Move {
        bool isTurn;
        union {
            Turn turn;
            CubeRotation cubeRotation;
        };

        explicit Move(const Turn &turn) : isTurn(true), turn(turn) {}

        explicit Move(const CubeRotation &cubeRotation) : isTurn(false), cubeRotation(cubeRotation) {}

        [[nodiscard]] Move inv() const;
    };

    std::vector<Move> moves;

    [[nodiscard]] size_t length() const;

    [[nodiscard]] std::string toStr() const;

    void cancelMoves();

    static Algorithm parse(const std::string &alg);

    [[nodiscard]] Algorithm inv() const;

    [[nodiscard]] Algorithm subAlgorithm(const size_t &start, const size_t &length) const;

    [[nodiscard]] Algorithm operator+(const Algorithm &other) const;

    [[nodiscard]] Algorithm operator*(const int &times) const;

    [[nodiscard]] Algorithm withSetup(const std::string &setup_alg_string) const;

    static Algorithm commutator(const std::string &first_alg_str, const std::string &second_alg_str);
};
