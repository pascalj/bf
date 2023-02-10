cmake_minimum_required(VERSION 3.0)

project(bf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_FLAGS "-march=native")

find_package(LLVM REQUIRED CONFIG)

add_executable(naive src/naive.cpp)
add_executable(llvm src/llvm.cpp)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories("include/")
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs support core)
target_link_libraries(llvm ${llvm_libs})
