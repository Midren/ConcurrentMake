#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <iostream>
#include <fstream>

std::string get_os(){
    std::string linux_dist;
    struct utsname name;
    if(uname(&name)) throw(std::runtime_error("Error while defining OS characteristics"));
    return std::string(name.sysname);
}

std::tuple<std::string, int, int> compiler_version(){
    std::string compiler;
    int minor = 0;
    int major = 0;
    #if defined(__clang__)
    /* Clang/LLVM. ---------------------------------------------- */
    compiler = std::string{__clang__};
    major = __clang_major__;
    minor = __clang_minor__;
    #elif defined(__ICC) || defined(__INTEL_COMPILER)
    /* Intel ICC/ICPC. ------------------------------------------ */
    compiler = std::string{"ICC"};
    major = __INTEL_COMPILER/100;
    minor = __INTEL_COMPILER%100;
    #elif defined(__GNUC__) || defined(__GNUG__)
    /* GNU GCC/G++. --------------------------------------------- */
    compiler = std::string{"GCC"};
    major = __GNUC__;
    minor = __GNUC_MINOR__;
    #elif defined(_MSC_VER)
    /* Microsoft Visual Studio. --------------------------------- */
    compiler = std::string{"MVS "}
    major = _MSC_VER/100;
    minor = _MSC_VER%100;    
    #endif
    return std::make_tuple(compiler, major, minor);
}


std::pair<std::string,std::string> get_public_key_and_login() {
    std::ifstream public_key_file{std::string(getenv("HOME")) + "/.ssh/id_rsa.pub"};
    std::string current, public_key, login;
    int iter = 0;
    while (std::getline(public_key_file, current, ' ')) {
        switch (iter) {
            case 1: {
                public_key = current;
                break;
            }
            case 2: {
                login = current.substr(0, current.find('@'));
                break;
            }
            default:
                break;

        }
        ++iter;
    }
    return make_pair(public_key, login);
}