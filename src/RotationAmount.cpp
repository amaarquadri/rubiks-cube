#include "RotationAmount.h"
#include <stdexcept>

RotationAmount inv(const RotationAmount &rotationAmount) {
    switch (rotationAmount) {
        case NONE:
            return NONE;
        case CLOCKWISE:
            return COUNTERCLOCKWISE;
        case HALF_TURN:
            return HALF_TURN;
        case COUNTERCLOCKWISE:
            return CLOCKWISE;
        default:
            throw std::logic_error("Unknown enum value!");
    }
}

std::string toStr(const RotationAmount &rotationAmount) {
    switch (rotationAmount) {
        case NONE:
        case CLOCKWISE:
            return "";
        case HALF_TURN:
            return "2";
        case COUNTERCLOCKWISE:
            return "'";
        default:
            throw std::logic_error("Unknown enum value!");
    }
}

RotationAmount operator+(const RotationAmount &first, const RotationAmount &second) {
    uint8_t clockwiseTurns = (static_cast<uint8_t>(first) + static_cast<uint8_t>(second)) % 4;
    return static_cast<RotationAmount>(clockwiseTurns);
}

std::pair<int, RotationAmount> parseRotationAmount(const std::string &str) {
    int rotationAmount = 1; // default to a single CLOCKWISE RotationAmount
    int consumed = 0;
    for (char chr : str) {
        if (chr >= '0' && chr <= '9') {
            int digit = chr - '0';
            rotationAmount = consumed == 0 ? digit % 4 : (2 * rotationAmount + digit) % 4;
            consumed++;
        }
        else if (chr == '\'') {
            rotationAmount = (4 - rotationAmount) % 4;
            consumed++;
            break;
        }
        else break;
    }

    switch (rotationAmount) {
        case 0:
            return {consumed, consumed == 0 ? CLOCKWISE : NONE};
        case 1:
            return {consumed, CLOCKWISE};
        case 2:
            return {consumed, HALF_TURN};
        case 3:
            return {consumed, COUNTERCLOCKWISE};
        default:
            throw std::runtime_error("Impossible result mod 4!");
    }
}
