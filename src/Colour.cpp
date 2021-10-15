#include "Colour.h"

Colour getOpposite(const Colour &colour) {
    switch (colour) {
        case WHITE:
            return YELLOW;
        case GREEN:
            return BLUE;
        case RED:
            return ORANGE;
        case BLUE:
            return GREEN;
        case ORANGE:
            return RED;
        case YELLOW:
            return WHITE;
    }
}

std::string toStr(const Colour &colour) {
    switch (colour) {
        case WHITE:
            return "W";
        case GREEN:
            return "G";
        case RED:
            return "R";
        case BLUE:
            return "B";
        case ORANGE:
            return "O";
        case YELLOW:
            return "Y";
    }
}
