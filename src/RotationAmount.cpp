#include "RotationAmount.h"

std::string toStr(const RotationAmount &rotationAmount) {
    switch (rotationAmount) {
        case CLOCKWISE:
            return "";
        case COUNTERCLOCKWISE:
            return "'";
        case HALF_TURN:
            return "2";
    }
}
