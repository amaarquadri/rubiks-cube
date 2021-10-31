#include "Cube.h"
#include "Permutation.h"
#include <chrono>
#include <random>
#include <stdexcept>

static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // NOLINT(cert-err58-cpp)
static auto random_engine = std::default_random_engine(seed); // NOLINT(cert-err58-cpp)
static std::uniform_int_distribution<uint8_t> bool_distribution(0, 1); // NOLINT(cert-err58-cpp)
static std::uniform_int_distribution<uint8_t> three_distribution(0, 2); // NOLINT(cert-err58-cpp)

EdgePiece Cube::getEdge(const EdgeLocation &edgeLocation) const {
    EdgeLocation flippedLocation = edgeLocation.flip();
    for (int i = 0; i < 12; i++) {
        if (EDGE_LOCATION_ORDER[i] == edgeLocation) return edges[i];
        if (EDGE_LOCATION_ORDER[i] == flippedLocation) return edges[i].flip();
    }
    throw std::invalid_argument("EdgeLocation not found!");
}

void Cube::setEdge(const EdgePiece &edgePiece, const EdgeLocation &edgeLocation) {
    EdgeLocation flippedLocation = edgeLocation.flip();
    for (int i = 0; i < 12; i++) {
        if (EDGE_LOCATION_ORDER[i] == edgeLocation) edges[i] = edgePiece;
        if (EDGE_LOCATION_ORDER[i] == flippedLocation) edges[i] = edgePiece.flip();
    }
}

CornerPiece Cube::getCorner(const CornerLocation &cornerLocation) const {
    CornerLocation clockwiseLocation = cornerLocation.rotateClockwise();
    CornerLocation counterclockwiseLocation = cornerLocation.rotateCounterClockwise();
    for (int i = 0; i < 8; i++) {
        if (CORNER_LOCATION_ORDER[i] == cornerLocation) return corners[i];
        if (CORNER_LOCATION_ORDER[i] == clockwiseLocation) return corners[i].rotateCounterclockwise();
        if (CORNER_LOCATION_ORDER[i] == counterclockwiseLocation) return corners[i].rotateClockwise();
    }
    throw std::invalid_argument("CornerLocation not found!");
}

void Cube::setCorner(const CornerPiece &cornerPiece, const CornerLocation &cornerLocation) {
    CornerLocation clockwiseLocation = cornerLocation.rotateClockwise();
    CornerLocation counterclockwiseLocation = cornerLocation.rotateCounterClockwise();
    for (int i = 0; i < 8; i++) {
        if (CORNER_LOCATION_ORDER[i] == cornerLocation) corners[i] = cornerPiece;
        if (CORNER_LOCATION_ORDER[i] == clockwiseLocation) corners[i] = cornerPiece.rotateClockwise();
        if (CORNER_LOCATION_ORDER[i] == counterclockwiseLocation) corners[i] = cornerPiece.rotateCounterclockwise();
    }
}


void Cube::apply(const Turn &turn) {
    if (turn.rotationAmount == NONE) return;

    std::array<EdgeLocation, 4> edgeCycle{};
    std::array<CornerLocation, 4> cornerCycle{};

    switch (turn.face) {
        case U:
            edgeCycle = {{
                                 {U, B}, {U, R},
                                 {U, F}, {U, L}}};
            cornerCycle = {{
                                   {U, L, B},
                                   {U, B, R},
                                   {U, R, F},
                                   {U, F, L}}};
            break;
        case F:
            edgeCycle = {{
                                 {F, U},
                                 {F, R},
                                 {F, D},
                                 {F, L}}};
            cornerCycle = {{
                                   {F, L, U},
                                   {F, U, R},
                                   {F, R, D},
                                   {F, D, L}}};
            break;
        case R:
            edgeCycle = {{
                                 {R, U},
                                 {R, B},
                                 {R, D},
                                 {R, F}}};
            cornerCycle = {{
                                   {R, F, U},
                                   {R, U, B},
                                   {R, B, D},
                                   {R, D, F}}};
            break;
        case B:
            edgeCycle = {{
                                 {B, U},
                                 {B, L},
                                 {B, D},
                                 {B, R}}};
            cornerCycle = {{
                                   {B, R, U},
                                   {B, U, L},
                                   {B, L, D},
                                   {B, D, R}}};
            break;
        case L:
            edgeCycle = {{
                                 {L, U},
                                 {L, F},
                                 {L, D},
                                 {L, B}}};
            cornerCycle = {{
                                   {L, B, U},
                                   {L, U, F},
                                   {L, F, D},
                                   {L, D, B}}};
            break;
        case D:
            edgeCycle = {{
                                 {D, F},
                                 {D, R},
                                 {D, B},
                                 {D, L}}};
            cornerCycle = {{
                                   {D, L, F},
                                   {D, F, R},
                                   {D, R, B},
                                   {D, B, L}}};
            break;
    }
    switch (turn.rotationAmount) {
        case CLOCKWISE:
            cycleEdges({edgeCycle[0], edgeCycle[1], edgeCycle[2], edgeCycle[3]});
            cycleCorners({cornerCycle[0], cornerCycle[1], cornerCycle[2], cornerCycle[3]});
            break;
        case COUNTERCLOCKWISE:
            cycleEdges({edgeCycle[0], edgeCycle[3], edgeCycle[2], edgeCycle[1]});
            cycleCorners({cornerCycle[0], cornerCycle[3], cornerCycle[2], cornerCycle[1]});
            break;
        case HALF_TURN:
            cycleEdges({edgeCycle[0], edgeCycle[2]});
            cycleEdges({edgeCycle[1], edgeCycle[3]});
            cycleCorners({cornerCycle[0], cornerCycle[2]});
            cycleCorners({cornerCycle[1], cornerCycle[3]});
            break;
    }
}

void Cube::apply(const CubeRotation &cubeRotation) {

}

void Cube::apply(const Algorithm &algorithm) {
    for (Algorithm::Move move: algorithm.moves) {
        if (move.isTurn) apply(move.turn);
        else apply(move.cubeRotation);
    }
}

void Cube::cycleEdges(const std::vector<EdgeLocation> &edgeLocations) {
    EdgePiece nextEdgePiece{};
    uint8_t i = 0;
    for (auto edgeLocation = edgeLocations.begin(); edgeLocation < edgeLocations.end(); edgeLocation++) {
        if (i == 0) {
            nextEdgePiece = getEdge(*edgeLocation);
            i++;
            continue;
        }
        EdgePiece temp = getEdge(*edgeLocation);
        setEdge(nextEdgePiece, *edgeLocation);
        nextEdgePiece = temp;
        i++;
    }
    setEdge(nextEdgePiece, *edgeLocations.begin());
}

void Cube::cycleCorners(const std::vector<CornerLocation> &cornerLocations) {
    CornerPiece nextCornerPiece{};
    uint8_t i = 0;
    for (auto cornerLocation = cornerLocations.begin(); cornerLocation < cornerLocations.end(); cornerLocation++) {
        if (i == 0) {
            nextCornerPiece = getCorner(*cornerLocation);
            i++;
            continue;
        }
        CornerPiece temp = getCorner(*cornerLocation);
        setCorner(nextCornerPiece, *cornerLocation);
        nextCornerPiece = temp;
        i++;
    }
    setCorner(nextCornerPiece, *cornerLocations.begin());
}

void Cube::scramble() {
    Permutation<12> edge_permutation = Permutation<12>::randomPermutation(random_engine);
    Permutation<8> corner_permutation = Permutation<8>::randomPermutation(random_engine);
    if (edge_permutation.isOdd() != corner_permutation.isOdd()) corner_permutation.flipParity();

    edges = edge_permutation.apply(STARTING_EDGE_PIECES);
    corners = corner_permutation.apply(STARTING_CORNER_PIECES);

    bool edgeFlipParity = false;
    for (int i = 0; i < 11; i++) {
        if (bool_distribution(random_engine) == 1) {
            edges[i] = edges[i].flip();
            edgeFlipParity = !edgeFlipParity;
        }
    }
    if (edgeFlipParity) edges[11] = edges[11].flip();

    uint8_t cornerRotationParity = 0;
    for (int i = 0; i < 7; i++) {
        uint8_t rotation = three_distribution(random_engine);
        if (rotation == 1) corners[i] = corners[i].rotateClockwise();
        else if (rotation == 2) corners[i] = corners[i].rotateCounterclockwise();
        cornerRotationParity += rotation;
    }
    uint8_t lastRotation = cornerRotationParity % 3;
    if (lastRotation == 2) corners[7] = corners[7].rotateClockwise(); // need another 1 to get to 3
    else if (lastRotation == 1) corners[7] = corners[7].rotateCounterclockwise(); // need another 2 to get to 3
}

bool Cube::edgesSolved() const {
    // TODO: account for different CubeOrientation
    return edges == STARTING_EDGE_PIECES;
}

bool Cube::cornersSolved() const {
    // TODO: account for different CubeOrientation
    return corners == STARTING_CORNER_PIECES;
}

bool Cube::isSolved() const {
    return edgesSolved() && cornersSolved();
}

std::string Cube::toNetString() const {
    //  B
    // LURD
    //  F
    return "    " + toStr(corners[7].second) + toStr(edges[10].second) + toStr(corners[6].third) +
           "\n    " + toStr(edges[6].first) + "B" + toStr(edges[5].second) +
           "\n    " + toStr(corners[0].third) + toStr(edges[0].second) + toStr(corners[1].second) +
           "\n" + toStr(corners[7].third) + toStr(edges[6].second) + toStr(corners[0].second) +
           " " + toStr(corners[0].first) + toStr(edges[0].first) + toStr(corners[1].first) +
           " " + toStr(corners[1].third) + toStr(edges[5].first) + toStr(corners[6].second) +
           " " + toStr(corners[6].first) + toStr(edges[10].first) + toStr(corners[7].first) +
           "\n" + toStr(edges[11].second) + "O" + toStr(edges[3].second) +
            " " + toStr(edges[3].first) + "W" + toStr(edges[1].first) +
            " " + toStr(edges[1].second) + "R" + toStr(edges[9].second) +
            " " + toStr(edges[9].first) + "Y" + toStr(edges[11].first) +
           "\n" + toStr(corners[4].second) + toStr(edges[7].first) + toStr(corners[3].third) +
            " " + toStr(corners[3].first) + toStr(edges[2].first) + toStr(corners[2].first) +
            " " + toStr(corners[2].second) + toStr(edges[4].second) + toStr(corners[5].third) +
            " " + toStr(corners[5].first) + toStr(edges[8].first) + toStr(corners[4].first) +
           "\n    " + toStr(corners[3].second) + toStr(edges[2].second) + toStr(corners[2].third) +
           "\n    " + toStr(edges[7].second) + "G" + toStr(edges[4].first) +
           "\n    " + toStr(corners[4].third) + toStr(edges[8].second) + toStr(corners[5].second);
}
