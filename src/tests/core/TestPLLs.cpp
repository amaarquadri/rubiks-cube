#include "TestPLLs.h"
#include "Cube.h"
#include "PLL.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>

void testPLLs() {
  const auto verify_alg = [](const Algorithm& alg, const size_t& iterations) {
    Cube cube{};
    for (size_t i = 0; i < iterations; i++) cube.apply(alg);
    if (!cube.isSolved())
      throw std::logic_error(
          "alg doesn't solve the Cube after the correct number of iterations.");
  };
  for (const Algorithm& alg :
       {E_PERM, H_PERM, Z_PERM, F_PERM, Ja_PERM, Jb_PERM, Na_PERM, Nb_PERM,
        Ra_PERM, Rb_PERM, T_PERM, V_PERM + Algorithm::parse("Y'"), Y_PERM})
    verify_alg(alg, 2);
  for (const Algorithm& alg : {Aa_PERM, Ab_PERM, Ua_PERM, Ub_PERM, Ga_PERM,
                               Gb_PERM + Algorithm::parse("Y'"), Gc_PERM,
                               Gd_PERM + Algorithm::parse("Y")})
    verify_alg(alg, 3);
  std::cout << "Passed all tests for PLLs!\n";
}
