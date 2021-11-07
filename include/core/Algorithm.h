#pragma once

#include "Move.h"
#include <utility>
#include <vector>
#include <string>

struct Algorithm {
    std::vector<Move> moves;

    Algorithm() = default;

    Algorithm(const Algorithm &other);

    explicit Algorithm(std::vector<Move> moves) : moves(std::move(moves)) {}

    Algorithm& operator=(const Algorithm &other);

    [[nodiscard]] size_t length() const;

    [[nodiscard]] std::string toStr() const;

    /**
     * @return True if applying this Algorithm, and then the inverse of the provided Algorithm on a solved Cube
     * results in a solved cube in the standard orientation.
     */
    [[nodiscard]] bool operator==(const Algorithm &other) const;

    [[nodiscard]] Move operator[](const size_t &index) const;

    [[nodiscard]] Move& operator[](const size_t &index);

    void push_back(const Move &move);

    void cancelMoves();

    static Algorithm parse(const std::string &alg);

    /**
     * @brief Parses the provided string, breaking apart all moves into half turns.
     */
    static Algorithm parseExpanded(const std::string &alg);

    /**
     * @brief Parses the given string into a scramble (which must be on the first line)
     * and the solve (which must be on subsequent lines).
     */
    static std::pair<Algorithm, Algorithm> parseScrambleSolve(const std::string &alg);

    [[nodiscard]] Algorithm inv() const;

    [[nodiscard]] Algorithm subAlgorithm(const size_t &start, const size_t &end) const;

    [[nodiscard]] Algorithm operator+(const Algorithm &other) const;

    [[nodiscard]] Algorithm operator*(const int &times) const;

    [[nodiscard]] Algorithm withSetup(const std::string &setup_alg_string) const;

    static Algorithm commutator(const std::string &first_alg_str, const std::string &second_alg_str);
};
