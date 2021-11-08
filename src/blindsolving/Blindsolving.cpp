#include "Blindsolving.h"
#include "Cube.h"
#include "PLL.h"
#include <algorithm>

namespace blindsolving {
    static std::unordered_map<EdgeLocation, char> getEdgeLettering() {
        std::unordered_map<EdgeLocation, char> edge_lettering;
        std::string edge_labels = "ABCDFJNRUVWZ";
        std::string flipped_edge_labels = "MIEQLPTHGKOS";
        for (size_t i = 0; i < Cube::EDGE_LOCATION_ORDER.size(); i++) {
            EdgeLocation location = Cube::EDGE_LOCATION_ORDER[i];
            edge_lettering.insert({location, edge_labels[i]});
            edge_lettering.insert({location.flip(), flipped_edge_labels[i]});
        }
        return edge_lettering;
    }

    static std::unordered_map<CornerLocation, char> getCornerLettering() {
        std::unordered_map<CornerLocation, char> corner_lettering;
        std::string corner_labels = "ABCDUVWZ";
        std::string clockwise_corner_labels = "NJFRHLPT";
        std::string counterclockwise_corner_labels = "QMIESGKO";
        for (size_t i = 0; i < Cube::CORNER_LOCATION_ORDER.size(); i++) {
            CornerLocation location = Cube::CORNER_LOCATION_ORDER[i];
            corner_lettering.insert({location, corner_labels[i]});
            corner_lettering.insert({location.rotateClockwise(), clockwise_corner_labels[i]});
            corner_lettering.insert({location.rotateCounterClockwise(), counterclockwise_corner_labels[i]});
        }
        return corner_lettering;
    }

    const std::unordered_map<EdgeLocation, char> EDGE_LETTERING = getEdgeLettering(); // NOLINT(cert-err58-cpp)
    const std::unordered_map<CornerLocation, char> CORNER_LETTERING = getCornerLettering(); // NOLINT(cert-err58-cpp)

    static const Algorithm A_ALG = Algorithm::parse("M2"); // NOLINT(cert-err58-cpp)
    static const Algorithm C_ALG = Algorithm::parse("U2 M' U2 M'"); // NOLINT(cert-err58-cpp)
    static const Algorithm E_ALG = Algorithm::parse("D M' U R2 U' M U R2 U' D' M2"); // NOLINT(cert-err58-cpp)
    const std::map<char, Algorithm> EDGE_ALGS = { // NOLINT(cert-err58-cpp)
            {'A', A_ALG},
            {'B', A_ALG.withSetup("R U R' U'")},
            {'C', C_ALG},
            {'D', A_ALG.withSetup("L' U' L U")},
            {'E', E_ALG},
            {'F', A_ALG.withSetup("U R U'")},
            {'H', A_ALG.withSetup("U' L' U")},
            {'I', A_ALG.withSetup("B' R B")},
            {'J', A_ALG.withSetup("R B' R' B")},
            {'K', A_ALG.withSetup("B' R' B")},
            {'L', A_ALG.withSetup("B' R2 B")},
            {'M', Algorithm::parse("U' M'") * 3 + Algorithm::parse("U' M") + Algorithm::parse("U' M'") * 4},
            {'N', A_ALG.withSetup("U' L U")},
            {'O', E_ALG.inv()},
            {'P', A_ALG.withSetup("U R' U'")},
            {'Q', A_ALG.withSetup("B L' B'")},
            {'R', A_ALG.withSetup("B L2 B'")},
            {'S', A_ALG.withSetup("B L B'")},
            {'T', A_ALG.withSetup("L' B L B'")},
            {'V', A_ALG.withSetup("U R2 U'")},
            {'W', C_ALG.inv()},
            {'Z', A_ALG.withSetup("U' L2 U")},
    };

    static const Algorithm L_ALG = Algorithm::parse("R U' R' U' R U R' F' R U R' U' R' F R"); // NOLINT(cert-err58-cpp)
    const std::map<char, Algorithm> CORNER_ALGS = { // NOLINT(cert-err58-cpp)
            {'B', Jb_PERM.withSetup("U'")},
            {'C', Y_PERM},
            {'D', Ja_PERM.withSetup("U2")},
            {'E', L_ALG.withSetup("F' D")},
            {'F', Jb_PERM.withSetup("R U'")},
            {'G', Y_PERM.withSetup("R")},
            {'H', L_ALG.withSetup("D")},
            {'I', L_ALG.withSetup("R'")},
            {'J', L_ALG.withSetup("R2")},
            {'K', L_ALG.withSetup("R")},
            {'L', L_ALG},
            {'M', Y_PERM.withSetup("R'")},
            {'O', L_ALG.withSetup("D' R")},
            {'P', L_ALG.withSetup("D'")},
            {'R', Y_PERM.withSetup("F")},
            {'S', Y_PERM.withSetup("D R")},
            {'T', L_ALG.withSetup("D2")},
            {'U', L_ALG.withSetup("F'")},
            {'V', Y_PERM.withSetup("D R2")},
            {'W', Y_PERM.withSetup("R2")},
            {'Z', Y_PERM.withSetup("D' R2")}};

    const Algorithm PARITY_ALG = A_ALG.withSetup("D' L2 D"); // NOLINT(cert-err58-cpp)

    Reconstruction parseSolveAttempt(const Algorithm &moves) {
        int consumed = 0;

        // extract all initial CubeRotations
        CubeOrientation orientation = CubeOrientation::identity();
        while (consumed < moves.length() && !moves[consumed].isTurn) {
            orientation *= moves[consumed].cubeRotation;
            consumed++;
        }
        for (size_t i = consumed + 1; i < moves.length(); i++) {
            if (!moves[i].isTurn) {
                throw std::invalid_argument("CubeRotations can only be at the start!");
            }
        }

        // set up transformations
        std::map<char, Cube> edge_alg_transformations;
        for (auto[chr, alg]: EDGE_ALGS) {
            Cube cube{};
            cube.apply(alg);
            edge_alg_transformations.insert({chr, cube});
        }
        std::map<char, Cube> corner_alg_transformations;
        for (auto[chr, alg]: CORNER_ALGS) {
            Cube cube{};
            cube.apply(alg);
            corner_alg_transformations.insert({chr, cube});
        }
        Cube parity_alg_transformation{};
        parity_alg_transformation.apply(PARITY_ALG);

        Reconstruction reconstruction;
        while (consumed < moves.length()) {
            bool found_match = false;
            SolveData solve_data{};
            Cube test_transformation{};
            for (int i = consumed; i < moves.length(); i++) {
                test_transformation.apply(orientation.apply(moves[i].turn));

                for (auto[chr, transformation]: edge_alg_transformations) {
                    if (test_transformation == transformation) {
                        found_match = true;
                        solve_data.moves = moves.subAlgorithm(consumed, i + 1);
                        solve_data.is_parsed = true;
                        solve_data.is_parity = false;
                        solve_data.is_edge = true;
                        solve_data.alg = chr;
                        break;
                    }
                }
                for (auto[chr, transformation]: corner_alg_transformations) {
                    if (test_transformation == transformation) {
                        found_match = true;
                        solve_data.moves = moves.subAlgorithm(consumed, i + 1);
                        solve_data.is_parsed = true;
                        solve_data.is_parity = false;
                        solve_data.is_edge = false;
                        solve_data.alg = chr;
                        break;
                    }
                }
                if (test_transformation == parity_alg_transformation) {
                    found_match = true;
                    solve_data.moves = moves.subAlgorithm(consumed, i + 1);
                    // solve_data is initialized to parity anyways, so no need to update here
                }
                if (found_match) {
                    reconstruction.solve_data.push_back(solve_data);
                    consumed = i + 1; // we consumed [0, i] which has a length of (i + 1)
                    break;
                }
            }
            if (!found_match) {
                if (reconstruction.solve_data.empty() || reconstruction[reconstruction.length() - 1].is_parsed) {
                    reconstruction.solve_data.emplace_back(Algorithm{});
                }
                reconstruction[reconstruction.length() - 1].moves.push_back(moves[consumed]);
                consumed++;
            }
        }
        return reconstruction;
    }

    constexpr static const std::array<EdgePiece, 12> PARITY_EDGE_PIECES{{
                                                                                Cube::STARTING_EDGE_PIECES[3],
                                                                                Cube::STARTING_EDGE_PIECES[1],
                                                                                Cube::STARTING_EDGE_PIECES[2],
                                                                                Cube::STARTING_EDGE_PIECES[0],
                                                                                Cube::STARTING_EDGE_PIECES[4],
                                                                                Cube::STARTING_EDGE_PIECES[5],
                                                                                Cube::STARTING_EDGE_PIECES[6],
                                                                                Cube::STARTING_EDGE_PIECES[7],
                                                                                Cube::STARTING_EDGE_PIECES[8],
                                                                                Cube::STARTING_EDGE_PIECES[9],
                                                                                Cube::STARTING_EDGE_PIECES[10],
                                                                                Cube::STARTING_EDGE_PIECES[11]}};

    /**
     * @return The EdgeLocation where the given EdgePiece belongs in a solved Cube.
     */
    EdgeLocation getLocation(const EdgePiece &edge_piece) {
        EdgePiece flipped_piece = edge_piece.flip();
        for (int i = 0; i < 12; i++) {
            if (edge_piece == Cube::STARTING_EDGE_PIECES[i]) {
                return Cube::EDGE_LOCATION_ORDER[i];
            } else if (flipped_piece == Cube::STARTING_EDGE_PIECES[i]) {
                return Cube::EDGE_LOCATION_ORDER[i].flip();
            }
        }
        throw std::invalid_argument("Could not find where the provided EdgePiece goes! It must be invalid.");
    }

    CornerLocation getLocation(const CornerPiece &corner_piece) {
        CornerPiece clockwise_piece = corner_piece.rotateClockwise();
        CornerPiece counterclockwise_piece = corner_piece.rotateCounterclockwise();
        for (int i = 0; i < 8; i++) {
            if (corner_piece == Cube::STARTING_CORNER_PIECES[i]) {
                return Cube::CORNER_LOCATION_ORDER[i];
            } else if (clockwise_piece == Cube::STARTING_CORNER_PIECES[i]) {
                return Cube::CORNER_LOCATION_ORDER[i].rotateCounterClockwise();
            }
            else if (counterclockwise_piece == Cube::STARTING_CORNER_PIECES[i]) {
                return Cube::CORNER_LOCATION_ORDER[i].rotateClockwise();
            }
        }
        throw std::invalid_argument("Could not find where the provided CornerPiece goes! It must be invalid.");
    }

    constexpr static const EdgeLocation EDGE_BUFFER = {D, F};
    constexpr static const CornerLocation CORNER_BUFFER = {U, L, B};

    std::vector<EdgeLocation> getUnsolvedEdges(const Cube &cube) {
        std::vector<EdgeLocation> unsolved_edge_locations;
        for (size_t i = 0; i < 12; i++) {
            if (Cube::EDGE_LOCATION_ORDER[i] == EDGE_BUFFER) continue; // skip the buffer piece itself
            if (cube.getEdge(Cube::EDGE_LOCATION_ORDER[i]) != Cube::STARTING_EDGE_PIECES[i]) {
                unsolved_edge_locations.push_back(Cube::EDGE_LOCATION_ORDER[i]);
            }
        }
        return unsolved_edge_locations;
    }

    std::vector<CornerLocation> getUnsolvedCorners(const Cube &cube) {
        std::vector<CornerLocation> unsolved_corner_locations;
        for (size_t i = 0; i < 8; i++) {
            if (Cube::CORNER_LOCATION_ORDER[i] == CORNER_BUFFER) continue; // skip the buffer piece itself
            if (cube.getCorner(Cube::CORNER_LOCATION_ORDER[i]) != Cube::STARTING_CORNER_PIECES[i]) {
                unsolved_corner_locations.push_back(Cube::CORNER_LOCATION_ORDER[i]);
            }
        }
        return unsolved_corner_locations;
    }

    char swapIfNecessary(const char &chr) {
        switch (chr) {
            case 'C':
                return 'W';
            case 'W':
                return 'C';
            case 'E':
                return 'O';
            case 'O':
                return 'E';
            default:
                return chr;
        }
    }

    std::unordered_map<Cube, std::vector<Reconstruction>> cache;
    std::vector<Reconstruction> getPossibleReconstructions(const Algorithm &scramble) {
        // set up test cube
        Cube cube{};
        cube.apply(scramble); // TODO: avoid re-scrambling every time

        // check the cache to see if we have calculated this scramble already
        auto it = cache.find(cube);
        if (it != cache.end()) {
            return it->second;
        }

        EdgePiece edge_buffer_piece = cube.getEdge(EDGE_BUFFER);
        if (getLocation(edge_buffer_piece) != EDGE_BUFFER && getLocation(edge_buffer_piece) != EDGE_BUFFER.flip()) {
            // edge_buffer_piece is not solved, nor flipped in the correct location
            EdgeLocation first_target = getLocation(edge_buffer_piece);
            EdgePiece next_piece = cube.getEdge(first_target);
            EdgeLocation second_target = getLocation(next_piece);

            char first_alg = EDGE_LETTERING.at(first_target);
            SolveData first = SolveData{EDGE_ALGS.at(first_alg), true, first_alg};

            if (second_target != EDGE_BUFFER && second_target.flip() != EDGE_BUFFER) {
                // normal letter pair
                char second_alg = swapIfNecessary(EDGE_LETTERING.at(second_target));
                SolveData second = SolveData{EDGE_ALGS.at(second_alg), true, second_alg};

                std::vector<Reconstruction> possible_reconstructions = getPossibleReconstructions(
                        scramble + first.moves + second.moves);
                for (Reconstruction &reconstruction: possible_reconstructions) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                }
                cache.insert({cube, possible_reconstructions});
                return possible_reconstructions;
            }

            // second target is buffer (or flipped buffer), so we need to break into a new cycle
            std::vector<EdgeLocation> unsolved_edges = getUnsolvedEdges(cube);
            unsolved_edges.erase(std::remove_if(unsolved_edges.begin(), unsolved_edges.end(),
                                                [&](const EdgeLocation &edge_location) {
                                                    return edge_location == first_target ||
                                                           edge_location == first_target.flip();
                                                }),
                                 unsolved_edges.end());
            if (unsolved_edges.empty()) {
                // letter, parity
                SolveData second = SolveData{};
                second.moves = PARITY_ALG;

                std::vector<Reconstruction> possible_reconstructions = getPossibleReconstructions(
                        scramble + first.moves + second.moves);
                for (Reconstruction &reconstruction: possible_reconstructions) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                }
                cache.insert({cube, possible_reconstructions});
                return possible_reconstructions;
            }
            // letter, new cycle
            std::vector<Reconstruction> possible_reconstructions;
            possible_reconstructions.reserve(2 * unsolved_edges.size());
            for (EdgeLocation &edge_location: unsolved_edges) {
                // we can have the edge_location as the second target
                char second_alg = swapIfNecessary(EDGE_LETTERING.at(edge_location));
                SolveData second = SolveData{EDGE_ALGS.at(second_alg), true, second_alg};
                for (Reconstruction reconstruction: getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }
                // or we can have the flipped side of the edge_location as the second target
                second_alg = swapIfNecessary(EDGE_LETTERING.at(edge_location.flip()));
                second = SolveData{EDGE_ALGS.at(second_alg), true, second_alg};
                for (Reconstruction reconstruction: getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }
            }
            cache.insert({cube, possible_reconstructions});
            return possible_reconstructions;
        }

        // edge buffer piece is already solved, or in the correct location but flipped
        std::vector<EdgeLocation> unsolved_edges = getUnsolvedEdges(cube);
        if (!unsolved_edges.empty() &&
            !(unsolved_edges.size() == 2 && unsolved_edges[0] == Cube::EDGE_LOCATION_ORDER[0] &&
              unsolved_edges[1] == Cube::EDGE_LOCATION_ORDER[3])) {
            // more edge cycles to break into
            std::vector<Reconstruction> possible_reconstructions;
            for (EdgeLocation &edge_location: unsolved_edges) {
                char first_alg = EDGE_LETTERING.at(edge_location);
                SolveData first = SolveData{EDGE_ALGS.at(first_alg), true, first_alg};
                EdgeLocation second_target = getLocation(cube.getEdge(edge_location));
                char second_alg = swapIfNecessary(EDGE_LETTERING.at(second_target));
                SolveData second = SolveData{EDGE_ALGS.at(second_alg), true, second_alg};
                for (Reconstruction reconstruction : getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }

                first_alg = EDGE_LETTERING.at(edge_location.flip());
                first = SolveData{EDGE_ALGS.at(first_alg), true, first_alg};
                second_target = second_target.flip();
                second_alg = swapIfNecessary(EDGE_LETTERING.at(second_target));
                second = SolveData{EDGE_ALGS.at(second_alg), true, second_alg};
                for (Reconstruction reconstruction : getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }
            }
            cache.insert({cube, possible_reconstructions});
            return possible_reconstructions;
        }

        // edges are fully solved, or solved up to parity
        CornerPiece corner_buffer_piece = cube.getCorner(CORNER_BUFFER);
        if (getLocation(corner_buffer_piece) != CORNER_BUFFER &&
            getLocation(corner_buffer_piece) != CORNER_BUFFER.rotateClockwise() &&
            getLocation(corner_buffer_piece) != CORNER_BUFFER.rotateCounterClockwise()) {
            // corner_buffer_piece is not solved, nor rotated in place
            CornerLocation first_target = getLocation(corner_buffer_piece);
            CornerPiece next_piece = cube.getCorner(first_target);
            CornerLocation second_target = getLocation(next_piece);

            char first_alg = CORNER_LETTERING.at(first_target);
            SolveData first = SolveData{CORNER_ALGS.at(first_alg), false, first_alg};

            if (second_target != CORNER_BUFFER && second_target.rotateClockwise() != CORNER_BUFFER &&
                                                  second_target.rotateCounterClockwise() != CORNER_BUFFER) {
                // normal letter pair
                char second_alg = CORNER_LETTERING.at(second_target);
                SolveData second = SolveData{CORNER_ALGS.at(second_alg), false, second_alg};

                std::vector<Reconstruction> possible_reconstructions = getPossibleReconstructions(
                        scramble + first.moves + second.moves);
                for (Reconstruction &reconstruction: possible_reconstructions) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                }
                cache.insert({cube, possible_reconstructions});
                return possible_reconstructions;
            }

            // second target is buffer (or one of its rotations), so we need to break into a new cycle
            std::vector<CornerLocation> unsolved_corners = getUnsolvedCorners(cube);
            unsolved_corners.erase(std::remove_if(unsolved_corners.begin(), unsolved_corners.end(),
                                                [&](const CornerLocation &corner_location) {
                                                    return corner_location == first_target ||
                                                           corner_location == first_target.rotateClockwise() ||
                                                           corner_location == first_target.rotateCounterClockwise();
                                                }),
                                 unsolved_corners.end());

            if (unsolved_corners.empty()) {
                // no second letter in this pair
                std::vector<Reconstruction> possible_reconstructions(1);
                possible_reconstructions[0].solve_data.push_back(first);
                cache.insert({cube, possible_reconstructions});
                return possible_reconstructions;
            }

            // letter, new cycle
            std::vector<Reconstruction> possible_reconstructions;
            possible_reconstructions.reserve(3 * unsolved_corners.size());
            for (CornerLocation &corner_location: unsolved_corners) {
                // we can have the corner_location as the second target
                char second_alg = CORNER_LETTERING.at(corner_location);
                SolveData second = SolveData{CORNER_ALGS.at(second_alg), false, second_alg};
                for (Reconstruction reconstruction: getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }
                // or we can have the clockwise rotation of the corner_location as the second target
                second_alg = CORNER_LETTERING.at(corner_location.rotateClockwise());
                second = SolveData{CORNER_ALGS.at(second_alg), false, second_alg};
                for (Reconstruction reconstruction: getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }
                // or we can have the counterclockwise rotation of the corner_location as the second target
                second_alg = CORNER_LETTERING.at(corner_location.rotateCounterClockwise());
                second = SolveData{CORNER_ALGS.at(second_alg), false, second_alg};
                for (Reconstruction reconstruction: getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }
            }
            cache.insert({cube, possible_reconstructions});
            return possible_reconstructions;
        }

        // corner buffer piece is already solved
        std::vector<CornerLocation> unsolved_corners = getUnsolvedCorners(cube);
        if (!unsolved_corners.empty()) {
            // more corner cycles to break into
            std::vector<Reconstruction> possible_reconstructions;
            for (CornerLocation &corner_location: unsolved_corners) {
                char first_alg = CORNER_LETTERING.at(corner_location);
                SolveData first = SolveData{CORNER_ALGS.at(first_alg), false, first_alg};
                CornerLocation second_target = getLocation(cube.getCorner(corner_location));
                char second_alg = CORNER_LETTERING.at(second_target);
                SolveData second = SolveData{CORNER_ALGS.at(second_alg), false, second_alg};
                for (Reconstruction reconstruction : getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }

                first_alg = CORNER_LETTERING.at(corner_location.rotateClockwise());
                first = SolveData{CORNER_ALGS.at(first_alg), false, first_alg};
                second_target = second_target.rotateClockwise();
                second_alg = CORNER_LETTERING.at(second_target);
                second = SolveData{CORNER_ALGS.at(second_alg), false, second_alg};
                for (Reconstruction reconstruction : getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }

                first_alg = CORNER_LETTERING.at(corner_location.rotateCounterClockwise());
                first = SolveData{CORNER_ALGS.at(first_alg), false, first_alg};
                second_target = second_target.rotateClockwise(); // rotate clockwise since we already rotated clockwise once already
                second_alg = CORNER_LETTERING.at(second_target);
                second = SolveData{CORNER_ALGS.at(second_alg), false, second_alg};
                for (Reconstruction reconstruction : getPossibleReconstructions(scramble + first.moves + second.moves)) {
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), second);
                    reconstruction.solve_data.insert(reconstruction.solve_data.begin(), first);
                    possible_reconstructions.push_back(reconstruction);
                }
            }
            cache.insert({cube, possible_reconstructions});
            return possible_reconstructions;
        }

        // cube is already solved
        std::vector<Reconstruction> possible_reconstructions(1);
        cache.insert({cube, possible_reconstructions});
        return possible_reconstructions;
    }

    /***
     * Computes the Levenshtein edit distance
     */
    unsigned int levEditDistance(const Reconstruction &first, const Reconstruction &second) {
        const size_t N = first.length();
        const size_t M = second.length();

        // ensure that M <= N, to reduce memory usage
        if (N < M) return levEditDistance(second, first);

        std::vector<unsigned int> current_row(M);
        for (size_t m = 0; m < M; m++) current_row[m] = m;

        for (size_t n = 1; n < N; n++) {
            unsigned int last_row_last_column = current_row[0];
            current_row[0] = n;
            for (size_t m = 1; m < M; m++) {
                if (first[n] != second[m]) {
                    // store the desired value of current_row[m] in last_row_last_column
                    if (current_row[m - 1] < last_row_last_column) last_row_last_column = current_row[m - 1];
                    if (current_row[m] < last_row_last_column) last_row_last_column = current_row[m];
                    last_row_last_column++;
                }
                std::swap(last_row_last_column, current_row[m]);
            }
        }
        return current_row[M - 1];

//        std::vector<unsigned int> last_row(M);
//        for (size_t m = 0; m < M; m++) last_row[m] = m;
//
//        for (size_t n = 1; n < N; n++) {
//            std::vector<unsigned int> current_row(M);
//            current_row[0] = n;
//            for (size_t m = 1; n < M; m++) {
//                if (first[n] == second[m]) {
//                    current_row[m] = last_row[m - 1];
//                }
//                else {
//                    unsigned int value = last_row[m - 1];
//                    if (last_row[m] < value) value = last_row[m];
//                    if (current_row[m - 1] < value) value = current_row[m - 1];
//                    current_row[m] = value + 1;
//                }
//            }
//        }
//
//        std::vector<unsigned int> matrix(N * M); // N rows, M columns, element (n, m) is at index (M * n + m)
//
//        // fill first row and first column
//        for (int m = 0; m < M; m++) matrix[m] = m;
//        for (int n = 1; n < N; n++) matrix[M * n] = n;
//
//        // recursively fill the rest of the matrix
//        for (int n = 1; n < N; n++) {
//            for (int m = 1; m < M; m++) {
//                    if (first[n] == second[m]) matrix[M * n + m] = matrix[M * (n - 1) + (m - 1)];
//                    else {
//                        // take 1 + the minimum of the three surrounding elements
//                        unsigned int value = matrix[M * (n - 1) + (m - 1)];
//                        if (matrix[M * n + (m - 1)] < value) value = matrix[M * n + (m - 1)];
//                        if (matrix[M * (n - 1) + m] < value) value = matrix[M * (n - 1) + m];
//                        matrix[M * n + m] = value + 1;
//                    }
//            }
//        }
//
//        // return last element of the matrix
//        return matrix[N * M - 1];
    }

    std::vector<unsigned int>
    sortBestReconstructions(const Reconstruction &solve, std::vector<Reconstruction> &possible_reconstructions) {
        size_t N = possible_reconstructions.size();
        if (N == 0) {
            throw std::invalid_argument("possible_reconstructions cannot be empty!");
        }

        std::vector<std::pair<Reconstruction, unsigned int>> options(N);
        for (int i = 0; i < N; i++) {
            options[i] = {possible_reconstructions[i], levEditDistance(solve, possible_reconstructions[i])};
        }
        std::sort(options.begin(), options.end(), [](const std::pair<Reconstruction, unsigned int> &first,
                                                     const std::pair<Reconstruction, unsigned int> &second) {
            return first.second < second.second;
        });

        std::vector<unsigned int> sorted_edit_distances(N);
        for (int i = 0; i < N; i++) {
            possible_reconstructions[i] = options[i].first;
            sorted_edit_distances[i] = options[i].second;
        }
        return sorted_edit_distances;
    }
}
