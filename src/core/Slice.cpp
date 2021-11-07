#include "Slice.h"
#include <stdexcept>

Face getRotationFace(const Slice &slice) {
    switch (slice) {
        case M:
            return L;
        case E:
            return D;
        case S:
            return F;
        default:
            throw std::logic_error("Unknown enum value!");
    }
}

std::pair<RotationAxis, bool> getRotationAxis(const Slice &slice) {
    switch (slice) {
        case M:
            return {X, true};
        case E:
            return {Y, true};
        case S:
            return {Z, false};
        default:
            throw std::logic_error("Unknown enum value!");
    }
}

std::pair<Slice, bool> fromRotationFace(const Face &face) {
    switch (face) {
        case L:
            return {M, false};
        case R:
            return {M, true};
        case D:
            return {E, false};
        case U:
            return {E, true};
        case F:
            return {S, false};
        case B:
            return {S, true};
        default:
            throw std::logic_error("Unknown enum value!");
    }
}

std::string toStr(const Slice &face) {
    switch (face) {
        case M:
            return "M";
        case E:
            return "E";
        case S:
            return "S";
        default:
            throw std::logic_error("Unknown enum value!");
    }
}

std::pair<int, Slice> parseSlice(const std::string &str) {
    if (str.empty()) return {0, {}};
    char first_char = str[0];
    if (first_char == 'M') return {1, M};
    else if (first_char == 'E') return {1, E};
    else if (first_char == 'S') return {1, S};
    else return {0, {}};
}
