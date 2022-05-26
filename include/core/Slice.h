#pragma once

#include "Face.h"
#include "RotationAxis.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

enum class Slice : uint8_t { M, E, S };

/**
 * @return The face that has the same rotation axis and direction as the given
 * Slice
 */
Face getRotationFace(const Slice& slice);

/**
 * @brief Computes the RotationAxis that results from applying this Slice with a
 * RotationAmount of Clockwise. If the bool is true, then the RotationAxis is in
 * the opposite direction
 */
std::pair<RotationAxis, bool> getRotationAxis(const Slice& slice);

/**
 * @breif Computes the slice that has the same rotation direction as the given
 * Face. If the bool is true, then the slice is the opposite direction
 */
std::pair<Slice, bool> fromRotationFace(const Face& face);

std::string toStr(const Slice& face);

/**
 * @brief Returns the number of characters consumed and the parsed Slice. If it
 * was not possible to parse a Slice, then the number of characters consumed
 * will be zero.
 */
std::pair<size_t, Slice> parseSlice(const std::string& str);
