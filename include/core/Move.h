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
};