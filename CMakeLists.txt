cmake_minimum_required(VERSION 3.16)
project(SP_3)

set(CMAKE_CXX_STANDARD 14)

add_executable(SP_3 src/Check.hpp src/Main.cpp src/SquareMatrix.hpp src/FindValue.hpp src/Common.hpp)

find_package(Threads REQUIRED)
link_libraries(Threads::Threads)
