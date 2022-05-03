#include "EdgeLocation.h"

bool EdgeLocation::operator==(const EdgeLocation& other) const {
  return first == other.first && second == other.second;
}

bool EdgeLocation::operator!=(const EdgeLocation& other) const {
  return !(*this == other);
}

EdgeLocation EdgeLocation::flip() const { return {second, first}; }
