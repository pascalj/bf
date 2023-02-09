cmake_minimum_required(VERSION 3.0)

project(bf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-march=native")

add_executable(naive naive.cpp)

