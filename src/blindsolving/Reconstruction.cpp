#include "Reconstruction.h"

namespace blindsolving {
    Reconstruction::Reconstruction(const Reconstruction &other) {
        *this = other;
    }

    Reconstruction& Reconstruction::operator=(const Reconstruction &other) {
        solve_data.resize(other.solve_data.size());
        std::copy(other.solve_data.begin(), other.solve_data.end(), solve_data.begin());
        return *this;
    }

    size_t Reconstruction::length() const {
        return solve_data.size();
    }

    SolveData Reconstruction::operator[](const size_t &index) const {
        // TODO: why is this constructor needed?
        return SolveData{solve_data[index]};
    }

    SolveData& Reconstruction::operator[](const size_t &index) {
        return solve_data[index];
    }
}
