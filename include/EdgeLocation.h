#pragma once

#include "Face.h"

struct EdgeLocation {
    Face first;
    Face second;

    bool operator==(const EdgeLocation &other) const;

    [[nodiscard]] EdgeLocation flip() const;
};
