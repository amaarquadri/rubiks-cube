#pragma once

#include "Algorithm.h"

const static Algorithm T_PERM = Algorithm::parse("RUR'U'R'FR2U'R'U'RUR'F'"); // NOLINT(cert-err58-cpp)
const static Algorithm Y_PERM = Algorithm::parse("FRU'R'U'RUR'F'RUR'U'R'FRF'"); // NOLINT(cert-err58-cpp)
const static Algorithm Ua_PERM = Algorithm::parse("R2U'R'U'RURURU'R"); // NOLINT(cert-err58-cpp)
const static Algorithm Ub_PERM = Ua_PERM.inv(); // NOLINT(cert-err58-cpp)
