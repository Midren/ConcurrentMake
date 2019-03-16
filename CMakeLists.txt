cmake_minimum_required(VERSION 3.12)
project(concurrent_make)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(libssh REQUIRED)

add_executable(concurrent_make main.cpp Scp.cpp Node.cpp)

target_link_libraries(concurrent_make -lssh)
