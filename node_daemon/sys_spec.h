

#ifndef CONCURRENT_MAKE_SYS_SPEC_H
#define CONCURRENT_MAKE_SYS_SPEC_H
#include <string>
#include <tuple>

std::tuple<std::string, std::string, std::string> compiler_version();
std::string get_os();
std::pair<std::string,std::string> get_public_key_and_login();



#endif //CONCURRENT_MAKE_SYS_SPEC_H