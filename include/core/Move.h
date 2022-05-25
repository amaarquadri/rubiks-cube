#pragma once

#include "CubeRotation.h"
#include "Turn.h"
#include <string>

struct Move {
  bool isTurn;
  union {
    Turn turn;
    CubeRotation cubeRotation;
  };

  Move() : isTurn(false), turn(Turn{}) {}

  Move(const Move& other);

  explicit Move(const Turn& turn) : isTurn(true), turn(turn) {}

  explicit Move(const CubeRotation& cubeRotation)
      : isTurn(false), cubeRotation(cubeRotation) {}

  Move& operator=(const Move& other);

  [[nodiscard]] Move inv() const;

  [[nodiscard]] std::string toStr() const;

  /**
   * @brief Returns the number of characters consumed and the parsed Move. If it
   * was not possible to parse a Move, then the number of characters consumed
   * will be zero.
   */
  static std::pair<size_t, Move> parse(const std::string& str);
};