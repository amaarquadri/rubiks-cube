#pragma once

#include "Face.h"

struct EdgeLocation {
    Face first;
    Face second;

    bool operator==(const EdgeLocation &other) const;

    [[nodiscard]] EdgeLocation flip() const;
};

namespace std {
    template<>
    struct hash<EdgeLocation> {
        size_t operator()(const EdgeLocation &edgeLocation) const {
            return 6 * static_cast<size_t>(edgeLocation.first) + static_cast<size_t>(edgeLocation.second);
        }
    };
}
