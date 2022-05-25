#include "Move.h"

Move::Move(const Move& other) { *this = other; }

Move& Move::operator=(const Move& other) {
  isTurn = other.isTurn;
  if (isTurn)
    turn = other.turn;
  else
    cubeRotation = other.cubeRotation;
  return *this;
}

Move Move::inv() const {
  if (isTurn) {
    return Move{turn.inv()};
  } else {
    return Move{cubeRotation.inv()};
  }
}

std::string Move::toStr() const {
  return isTurn ? turn.toStr() : cubeRotation.toStr();
}
