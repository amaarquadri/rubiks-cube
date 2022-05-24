#pragma once

#include "Algorithm.h"

const Algorithm T_PERM =
    Algorithm::parse("R U R' U' R' F R2 U' R' U' R U R' F'");
const Algorithm Y_PERM =
    Algorithm::parse("F R U' R' U' R U R' F' R U R' U' R' F R F'");
const Algorithm Ua_PERM = Algorithm::parse("R2 U' R' U' R U R U R U' R");
const Algorithm Ub_PERM = Ua_PERM.inv();

const Algorithm Ja_PERM = Algorithm::parse("L' U2 L U L' U2 R U' L U R'");
const Algorithm Jb_PERM =
    Algorithm::parse("R U R' F' R U R' U' R' F R2 U' R' U'");

const Algorithm Aa_PERM = Algorithm::parse("R' F R' B2 R F' R' B2 R2");
const Algorithm Ab_PERM = Aa_PERM.inv();
const Algorithm F_PERM =
    Algorithm::parse("R' U' F' R U R' U' R' F R2 U' R' U' R U R' U R");
const Algorithm E_PERM = Algorithm::commutator("R B' R'", "F") +
                         Algorithm::commutator("R B R'", "F");
const Algorithm H_PERM = Algorithm::parse("M2 U' M2 U2 M2 U' M2");
const Algorithm Z_PERM = Algorithm::parse("M2 U' M2 U' M' U2 M2 U2 M' U2");
const Algorithm V_PERM =
    Algorithm::parse("R' U R' U' Y R' F' R2 U' R' U R' F R F");

const Algorithm Ga_PERM =
    Algorithm::parse("R2 U R' U R' U' R U' R2 D U' R' U R D' U'");
const Algorithm Gb_PERM =
    Algorithm::parse("R' U' R Y R2 D Y R' U R U' R D' Y' R2");
const Algorithm Gc_PERM =
    Algorithm::parse("R2 F2 R U2 R U2 R' F R U R' U' R' F R2");
const Algorithm Gd_PERM =
    Algorithm::parse("R U R' Y' R2 D' Y' R U' R' U R' D Y R2");

const Algorithm Ra_PERM =
    Algorithm::parse("R U' R' U' R U R D R' U' R D' R' U2 R' U'");
const Algorithm Rb_PERM =
    Algorithm::parse("R' U2 R U2 R' F R U R' U' R' F' R2 U'");

const Algorithm Na_PERM = Jb_PERM.withSetup("R U R' U");
const Algorithm Nb_PERM = Ja_PERM.withSetup("L' U' L U'");
