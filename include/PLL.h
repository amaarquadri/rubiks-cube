#pragma once

#include "Algorithm.h"

const Algorithm T_PERM = Algorithm::parse("R U R' U' R' F R2 U' R' U' R U R' F'"); // NOLINT(cert-err58-cpp)
const Algorithm Y_PERM = Algorithm::parse("F R U' R' U' R U R' F' R U R' U' R' F R F'"); // NOLINT(cert-err58-cpp)
const Algorithm Ua_PERM = Algorithm::parse("R2 U' R' U' R U R U R U' R"); // NOLINT(cert-err58-cpp)
const Algorithm Ub_PERM = Ua_PERM.inv(); // NOLINT(cert-err58-cpp)

// TODO: check which one is which
const Algorithm Ja_PERM = Algorithm::parse("R U R' F' R U R' U' R' F R2 U' R' U'"); // NOLINT(cert-err58-cpp)
const Algorithm Jb_PERM = Algorithm::parse("L' U2 L' U L' U2 R U' L U R'"); // NOLINT(cert-err58-cpp)

// TODO: check which one is which
const Algorithm Aa_PERM = Algorithm::parse("R' F R' B2 R F' R' B2 R2"); // NOLINT(cert-err58-cpp)
const Algorithm Ab_PERM = Aa_PERM.inv(); // NOLINT(cert-err58-cpp)
const Algorithm F_PERM = Algorithm::parse("R' U' F' R U R' U' R' F R2 U' R' U' R U R' U R"); // NOLINT(cert-err58-cpp)
const Algorithm E_PERM = Algorithm::commutator("R B' R'", "D") + Algorithm::commutator("R B R'", "D"); // NOLINT(cert-err58-cpp)
const Algorithm H_PERM = Algorithm::parse("M2 U' M2 U2 M2 U' M2"); // NOLINT(cert-err58-cpp)
const Algorithm Z_PERM = Algorithm::parse("M2 U' M2 U' M' U2 M2 U2 M' U2"); // NOLINT(cert-err58-cpp)
const Algorithm V_PERM = Algorithm::parse("R' U R' U' X "); // NOLINT(cert-err58-cpp)

// TODO: check which one is which
const Algorithm Ga_PERM = Algorithm::parse(""); // NOLINT(cert-err58-cpp)
const Algorithm Gb_PERM = Algorithm::parse(""); // NOLINT(cert-err58-cpp)
const Algorithm Gc_PERM = Algorithm::parse(""); // NOLINT(cert-err58-cpp)
const Algorithm Gd_PERM = Algorithm::parse(""); // NOLINT(cert-err58-cpp)

// TODO: check which one is which
const Algorithm Na_PERM = Ja_PERM.withSetup("R U R' U"); // NOLINT(cert-err58-cpp)
const Algorithm Nb_PERM = Jb_PERM.withSetup("L' U' L U'"); // NOLINT(cert-err58-cpp)
