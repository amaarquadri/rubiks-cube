#include "Algorithm.h"
#include "Face.h"
#include "CubeOrientation.h"
#include <algorithm>

Algorithm::Move Algorithm::Move::inv() const {
    if (isTurn) {
        return Move{turn.inv()};
    }
    else {
        return Move{cubeRotation.inv()};
    }
}

size_t Algorithm::length() const {
    return moves.size();
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

static int mergeTurns(std::vector<Algorithm::Move> &moves, std::vector<std::pair<int, CubeOrientation>> &previousTurns) {
    // merges any turns resulting from the last element in previousTurns, and updates both vectors
    // returns the number of moves that were cancelled
    if (previousTurns.size() < 2) return 0;

    std::pair<int, CubeOrientation> last_pair = previousTurns[previousTurns.size() - 1];
    std::pair<int, CubeOrientation> second_last_pair = previousTurns[previousTurns.size() - 2];
    Turn original_last_turn = last_pair.second.apply(moves[last_pair.first].turn); // equivalent last turn from original CubeRotation
    Turn original_second_last_turn = second_last_pair.second.apply(moves[second_last_pair.first].turn); // equivalent second last turn from original CubeRotation

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
        std::pair<int, CubeOrientation> third_last_pair = previousTurns[previousTurns.size() - 3];
        Turn original_third_last_turn = third_last_pair.second.apply(moves[third_last_pair.first].turn); // equivalent third last turn from original CubeRotation
        if (original_last_turn.face == original_third_last_turn.face) {
            // temporarily remove second_last_pair, recurse to combine the last and 3rd last pairs, then add it back
            previousTurns.erase(previousTurns.end() - 2);
            int merged_turns = mergeTurns(moves, previousTurns);
            previousTurns.push_back(second_last_pair);
            return merged_turns;
        }
    }
    return 0;
}

void Algorithm::cancelMoves() {
    if (moves.empty()) return;

    // first pass, combine any adjacent CubeRotations with the same RotationAxis
    for (auto it = moves.begin() + 1; it < moves.end();) {
        if (!(it - 1)->isTurn && !it->isTurn && (it - 1)->cubeRotation.rotationAxis == it->cubeRotation.rotationAxis) {
            (it - 1)->cubeRotation.rotationAmount = (it - 1)->cubeRotation.rotationAmount + it->cubeRotation.rotationAmount;
            it = moves.erase(it);
        }
        else it++;
    }

    std::vector<std::pair<int, CubeOrientation>> previousTurns; // index, CubeOrientation at index
    CubeOrientation orientation = CubeOrientation::identity();
    for (int index = 0; index < moves.size(); index++) {
        Move move = moves[index];
        if (move.isTurn) {
            previousTurns.emplace_back(index, orientation);
            index -= mergeTurns(moves, previousTurns);
        }
        else {
            orientation *= move.cubeRotation;
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

Algorithm Algorithm::subAlgorithm(const size_t &start, const size_t &length) const {
    std::vector<Move> sub_moves(length);
    std::copy(moves.begin() + start, moves.begin() + start + length, sub_moves);
    return Algorithm{sub_moves};
}

Algorithm Algorithm::operator+(const Algorithm &other) const {
    std::vector<Move> sum_moves(moves.size() + other.moves.size());
    auto it = std::copy(moves.begin(), moves.end(), sum_moves.begin());
    std::copy(other.moves.begin(), other.moves.end(), it);
    return {sum_moves};
}

Algorithm Algorithm::operator*(const int &times) const {
    std::vector<Move> repeated_moves(moves.size() * times);
    auto it = repeated_moves.begin();
    for (int i = 0; i < times; i++) {
        it = std::copy(moves.begin(), moves.end(), it);
    }
    return Algorithm{repeated_moves};
}

Algorithm Algorithm::withSetup(const std::string &setup_alg_string) const {
    Algorithm setup_alg = Algorithm::parse(setup_alg_string);
    return setup_alg + (*this) + setup_alg.inv();
}

Algorithm Algorithm::commutator(const std::string &first_alg_str, const std::string &second_alg_str) {
    Algorithm first_alg = Algorithm::parse(first_alg_str);
    Algorithm second_alg = Algorithm::parse(second_alg_str);
    return first_alg + second_alg + first_alg.inv() + second_alg.inv();
}
