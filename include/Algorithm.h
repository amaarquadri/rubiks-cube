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

        [[nodiscard]] Move inv() const;
    };

    std::vector<Move> moves;

    [[nodiscard]] std::string toStr() const;

    void cancelMoves();

    static Algorithm parse(const std::string &alg);

    [[nodiscard]] Algorithm inv() const;
};
