#include <iostream>
#include "PLL.h"
#include "Cube.h"

int main() {
    Cube cube{};
    cube.apply(Ua_PERM);
    std::cout << cube.isSolved() << std::endl;
    cube.apply(Ub_PERM);
    std::cout << cube.isSolved() << std::endl;
    return 0;
}
