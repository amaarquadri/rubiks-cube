#pragma once

#include "Algorithm.h"

const static Algorithm T_PERM = Algorithm::parse("R U R' U' R' F R2 U' R' U' R U R' F'"); // NOLINT(cert-err58-cpp)
const static Algorithm Y_PERM = Algorithm::parse("F R U' R' U' R U R' F' R U R' U' R' F R F'"); // NOLINT(cert-err58-cpp)
const static Algorithm Ua_PERM = Algorithm::parse("R2 U' R' U' R U R U R U' R"); // NOLINT(cert-err58-cpp)
const static Algorithm Ub_PERM = Ua_PERM.inv(); // NOLINT(cert-err58-cpp)
