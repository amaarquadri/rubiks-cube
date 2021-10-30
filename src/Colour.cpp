#include "Colour.h"
#include "Cube.h"

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

Colour getLeft(const Colour &top, const Colour &front) {
    for (CornerPiece corner : Cube::STARTING_CORNER_PIECES) {
        if (top == corner.first && front == corner.second) return corner.third;
    }
    throw std::invalid_argument("Provided top and front colours are inconsistent!");
}

Colour getRight(const Colour &top, const Colour &front) {
    return getOpposite(getLeft(top, front));
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
