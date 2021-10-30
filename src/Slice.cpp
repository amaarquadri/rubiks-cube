#include "Slice.h"

std::string toStr(const Slice &face) {
    switch (face) {
        case M:
            return "M";
        case E:
            return "E";
        case S:
            return "S";
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
