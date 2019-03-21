cmake_minimum_required(VERSION 3.12)
project(concurrent_make)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(libssh REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(concurrent_make main.cpp Scp.cpp Node.cpp server_communication.cpp server_communnication.h)

target_link_libraries(concurrent_make ${CMAKE_THREAD_LIBS_INIT} ${Boost_SYSTEM_LIBRARIES})
target_link_libraries(concurrent_make -lssh stdc++fs)
