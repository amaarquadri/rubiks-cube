#pragma once

#include "Move.h"
#include <string>
#include <utility>
#include <vector>

struct Algorithm : public std::vector<Move> {
  Algorithm() = default;

  explicit Algorithm(const Algorithm::size_type& size)
      : std::vector<Move>(size) {}

  /**
   * Creates a random Algorithm with the given size consisting solely of Turns.
   */
  static Algorithm random(const size_t& size);

  [[nodiscard]] std::string toStr() const;

  /**
   * @return True if applying this Algorithm, and then the inverse of the
   * provided Algorithm on a solved Cube results in a solved cube in the
   * standard orientation.
   */
  [[nodiscard]] bool operator==(const Algorithm& other) const;

  [[nodiscard]] bool operator!=(const Algorithm& other) const;

  /**
   * Converts this Algorithm to standard form which consists entirely of Turns
   * with the exception of at most 2 CubeRotations at the start.
   */
  void toStandardForm();

  [[nodiscard]] bool isStandardForm() const;

  /**
   * Note that this converts this Algorithm to standard form.
   */
  void cancelMoves();

  static Algorithm parse(const std::string& alg);

  /**
   * @brief Parses the provided string, breaking apart all moves into quarter
   * turns.
   */
  static Algorithm parseExpanded(const std::string& alg);

  [[nodiscard]] Algorithm inv() const;

  [[nodiscard]] Algorithm subAlgorithm(const size_t& start,
                                       const size_t& end) const;

  [[nodiscard]] Algorithm operator+(const Algorithm& other) const;

  [[nodiscard]] Algorithm operator*(const size_t& times) const;

  [[nodiscard]] Algorithm withSetup(const std::string& setup_alg_string) const;

  static Algorithm commutator(const std::string& first_alg_str,
                              const std::string& second_alg_str);
};
