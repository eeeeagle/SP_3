cmake_minimum_required(VERSION 3.24)
project(SP_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(SP_3 src/main.cpp src/SquareMatrix.hpp src/check.hpp)

find_package(Threads REQUIRED)
link_libraries(Threads::Threads)
