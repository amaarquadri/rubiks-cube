#include "Algorithm.h"

std::string Algorithm::toStr() const {
    std::string result;
    for (auto move = moves.begin(); move < moves.end(); move++) {
        if (move->isTurn) result += move->turn.toStr();
        else result += move->cubeRotation.toStr();
    }
    return result;
}

Algorithm Algorithm::parse(const std::string &alg) {
    // TODO
    return {};
}
