#pragma once

#include "CubeRotation.h"
#include "SliceTurn.h"
#include "Turn.h"
#include "WideTurn.h"
#include <string>
#include <variant>

struct Move : public std::variant<Turn, SliceTurn, WideTurn, CubeRotation> {
  [[nodiscard]] bool isTurn() const;

  [[nodiscard]] bool isSliceTurn() const;

  [[nodiscard]] bool isWideTurn() const;

  [[nodiscard]] bool isCubeRotation() const;

  [[nodiscard]] const Turn& getTurn() const;

  [[nodiscard]] Turn& getTurn();

  [[nodiscard]] const SliceTurn& getSliceTurn() const;

  [[nodiscard]] SliceTurn& getSliceTurn();

  [[nodiscard]] const WideTurn& getWideTurn() const;

  [[nodiscard]] WideTurn& getWideTurn();

  [[nodiscard]] const CubeRotation& getCubeRotation() const;

  [[nodiscard]] CubeRotation& getCubeRotation();

  [[nodiscard]] Move inv() const;

  [[nodiscard]] std::string toStr() const;

  /**
   * @brief Returns the number of characters consumed and the parsed Move. If it
   * was not possible to parse a Move, then the number of characters consumed
   * will be zero.
   */
  static std::pair<size_t, Move> parse(const std::string& str);
};