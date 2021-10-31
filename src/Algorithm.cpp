#include "Algorithm.h"
#include "Face.h"
#include <algorithm>

Algorithm::Move Algorithm::Move::inv() const {
    if (isTurn) {
        return Move{turn.inv()};
    }
    else {
        return Move{cubeRotation.inv()};
    }
}

std::string Algorithm::toStr() const {
    std::string result;
    for (auto move = moves.begin(); move < moves.end(); move++) {
        if (move->isTurn) result += move->turn.toStr();
        else result += move->cubeRotation.toStr();
        result += " ";
    }
    return result;
}

static int mergeTurns(std::vector<Algorithm::Move> &moves, std::vector<std::pair<int, CubeRotation>> &previousTurns) {
    // merges any turns resulting from the last element in previousTurns, and updates both vectors
    // returns the number of moves that were cancelled
    if (previousTurns.size() < 2) return 0;

    std::pair<int, CubeRotation> last_pair = previousTurns[previousTurns.size() - 1];
    std::pair<int, CubeRotation> second_last_pair = previousTurns[previousTurns.size() - 2];
    Turn original_last_turn = last_pair.second.inv() * moves[last_pair.first].turn; // equivalent last turn from original CubeRotation
    Turn original_second_last_turn = second_last_pair.second.inv() * moves[second_last_pair.first].turn; // equivalent second last turn from original CubeRotation

    if (original_last_turn.face == original_second_last_turn.face) {
        RotationAmount new_rotation_amount = original_second_last_turn.rotationAmount + original_last_turn.rotationAmount;
        if (new_rotation_amount == NONE) {
            // remove both moves from moves and previousTurns
            moves.erase(moves.begin() + last_pair.first);
            moves.erase(moves.begin() + second_last_pair.first);
            previousTurns.erase(previousTurns.end() - 1);
            previousTurns.erase(previousTurns.end() - 1);
            return 2;
        }
        else {
            // update moves
            moves[second_last_pair.first].turn.rotationAmount = new_rotation_amount;
            // remove second move from moves and previousTurns
            moves.erase(moves.begin() + last_pair.first);
            previousTurns.erase(previousTurns.end() - 1);
            return 1;
        }
    }
    else if (getOpposite(original_last_turn.face) == original_second_last_turn.face && previousTurns.size() >= 3) {
        std::pair<int, CubeRotation> third_last_pair = previousTurns[previousTurns.size() - 3];
        Turn original_third_last_turn = third_last_pair.second.inv() * moves[third_last_pair.first].turn; // equivalent third last turn from original CubeRotation
        if (original_last_turn.face == original_third_last_turn.face) {
            // temporarily remove second_last_pair, recurse to combine the last and 3rd last pairs, then add it back
            previousTurns.erase(previousTurns.end() - 2);
            mergeTurns(moves, previousTurns);
            previousTurns.insert(previousTurns.end(), 1, second_last_pair);
            return 1;
        }
    }
    return 0;
}

void Algorithm::cancelMoves() {
    if (moves.empty()) return;

    // first pass, combine any adjacent CubeRotations
    for (auto it = moves.begin() + 1; it < moves.end();) {
        if (!(it - 1)->isTurn && !it->isTurn) {
            (it - 1)->cubeRotation = (it - 1)->cubeRotation * it->cubeRotation;
            it = moves.erase(it);
        }
        else it++;
    }

    std::vector<std::pair<int, CubeRotation>> previousTurns; // index, net CubeRotation at index
    CubeRotation currentRotation = CubeRotation::identity(); // rotation from original CubeOrientation to CubeOrientation at index
    for (int index = 0; index < moves.size(); index++) {
        Move move = moves[index];
        if (move.isTurn) {
            previousTurns.insert(previousTurns.end(), 1, {index, currentRotation});
            index -= mergeTurns(moves, previousTurns);
        }
        else {
            currentRotation = currentRotation * move.cubeRotation;
        }
    }
}

static int consumeSeparators(const std::string &alg) {
    int consumed = 0;
    while (consumed < alg.size()) {
        switch (alg[consumed]) {
            case ' ':
            case '\n':
                consumed++;
                break;
            case '/':
                while (alg[consumed] != '\n') {
                    consumed++;
                }
                consumed++; // consume the new line character too
                break;
            default:
                return consumed;
        }
    }
    return consumed; // consumed entire alg
}

Algorithm Algorithm::parse(const std::string &alg) {
    int total_consumed = 0;
    std::vector<Move> moves;
    while (total_consumed < alg.size()) {
        total_consumed += consumeSeparators(alg.substr(total_consumed, alg.size() - total_consumed));
        auto [consumed_for_turn, turn] = Turn::parse(alg.substr(total_consumed, alg.size() - total_consumed));
        if (consumed_for_turn == 0) {
            // couldn't parse a Turn, try parsing a CubeRotation instead
            auto [consumed_for_cube_rotation, cubeRotation] =
                    CubeRotation::parse(alg.substr(total_consumed, alg.size() - total_consumed));
            if (consumed_for_cube_rotation == 0) break;
            moves.emplace_back(cubeRotation);
            total_consumed += consumed_for_cube_rotation;
        } else {
            moves.emplace_back(turn);
            total_consumed += consumed_for_turn;
        }
    }
    return {moves};
}

Algorithm Algorithm::inv() const {
    std::vector<Move> inverse_moves(moves.size());
    std::transform(moves.rbegin(), moves.rend(), inverse_moves.begin(),
                   [](const Move &move) { return move.inv(); });
    return Algorithm{inverse_moves};
}
