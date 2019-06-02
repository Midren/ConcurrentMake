#include <iostream>
#include "ccmake.h"

int main() {
    auto args = getCcmakeArgs("../CCmake.txt");
    std::cout << args->exe << " " << args->sources << std::endl;
}

