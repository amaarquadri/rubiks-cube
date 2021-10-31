#pragma once

#include "Algorithm.h"

const static Algorithm T_PERM = Algorithm::parse("R U R' U' R' F R2 U' R' U' R U R' F'"); // NOLINT(cert-err58-cpp)
const static Algorithm Y_PERM = Algorithm::parse("F R U' R' U' R U R' F' R U R' U' R' F R F'"); // NOLINT(cert-err58-cpp)
const static Algorithm Ua_PERM = Algorithm::parse("R2 U' R' U' R U R U R U' R"); // NOLINT(cert-err58-cpp)
const static Algorithm Ub_PERM = Ua_PERM.inv(); // NOLINT(cert-err58-cpp)

// TODO: check which one is which
const static Algorithm Ja_PERM = Algorithm::parse("R U R' F' R U R' U' R' F R2 U' R' U'");
const static Algorithm Jb_PERM = Algorithm::parse("L' U2 L' U L' U2 R U' L U R'");

// TODO: check which one is which
const static Algorithm Aa_PERM = Algorithm::parse("R' F R' B2 R F' R' B2 R2");
const static Algorithm Ab_PERM = Aa_PERM.inv();
