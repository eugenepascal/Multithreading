cmake_minimum_required(VERSION 3.25)
project(TD_OpenMP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER "/usr/local/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
