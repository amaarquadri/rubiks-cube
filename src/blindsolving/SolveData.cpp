#include "SolveData.h"

namespace blindsolving {
    SolveData::SolveData(const SolveData &other) {
        *this = other;
    }

    bool SolveData::operator==(const SolveData &other) const {
        if (is_parsed != other.is_parsed) return false;
        if (!is_parsed) return moves == other.moves;
        // otherwise, both must be parsed

        if (is_parity != other.is_parity) return false;
        if (is_parity) return true;
        // otherwise, neither are parity

        if (is_edge != other.is_edge) return false;
        // otherwise, both must be the same piece type

        return alg == other.alg;
    }

    bool SolveData::operator!=(const SolveData &other) const {
        return !(*this == other);
    }

    std::string SolveData::toStr() const {
        if (is_parsed) {
            if (is_parity) return "Parity";
            else return (is_edge ? "Edge: " : "Corner: ") + std::string(1, alg);
        } else {
            return "Unknown Moves: " + moves.toStr();
        }
    }
}
