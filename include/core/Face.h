#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

enum class Face : uint8_t { U, F, R, B, L, D };

Face getOpposite(const Face& face);

Face getLeft(const Face& top, const Face& front);

Face getRight(const Face& top, const Face& right);

std::string toStr(const Face& face);

/**
 * @brief Returns the number of characters consumed and the parsed Face. If it
 * was not possible to parse a Face, then the number of characters consumed will
 * be zero.
 */
std::pair<size_t, Face> parseFace(const std::string& str);
