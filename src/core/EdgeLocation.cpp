#include "EdgeLocation.h"

bool EdgeLocation::operator==(const EdgeLocation &other) const {
    return first == other.first && second == other.second;
}

EdgeLocation EdgeLocation::flip() const {
    return {second, first};
}
