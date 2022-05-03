#pragma once

#include "CornerLocation.h"
#include "EdgeLocation.h"
#include <unordered_map>

namespace blindsolving {
extern const std::unordered_map<EdgeLocation, char> EDGE_LETTERING;
extern const std::unordered_map<CornerLocation, char> CORNER_LETTERING;
extern const std::unordered_map<char, EdgeLocation> REVERSE_EDGE_LETTERING;
extern const std::unordered_map<char, CornerLocation> REVERSE_CORNER_LETTERING;

char flipEdge(const char& edge);

char rotateClockwise(const char& corner);

char rotateCounterClockwise(const char& corner);
}  // namespace blindsolving
