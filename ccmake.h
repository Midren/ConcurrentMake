#ifndef CONCURRENT_MAKE_CCMAKE_H
#define CONCURRENT_MAKE_CCMAKE_H
#include <string>

struct Attributes {
    std::string exe;
    std::string sources;
    std::string headers;
    std::string flags;
};

Attributes *getCcmakeArgs(const std::string filename);

#endif //CONCURRENT_MAKE_CCMAKE_H
