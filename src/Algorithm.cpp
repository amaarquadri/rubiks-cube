#include "Algorithm.h"
#include "Face.h"

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

Algorithm Algorithm::parse(const std::string &alg) {
    // TODO : allow for parsing CubeRotations
    int total_consumed = 0;
    std::vector<Move> moves;
    while (total_consumed < alg.size()) {
        auto [consumed, turn] = Turn::parse(alg.substr(total_consumed, alg.size() - total_consumed));
        if (consumed == 0) break;
        moves.insert(moves.end(), 1, {true, turn});
        total_consumed += consumed;
    }
    return {moves};
}

Algorithm Algorithm::inv() const {
    std::vector<Move> inverse_moves;
    for (auto it = moves.end() - 1; it >= moves.begin(); it--)
        inverse_moves.insert(inverse_moves.end(), 1, it->inv());
    return {inverse_moves};
}
