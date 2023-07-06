cmake_minimum_required(VERSION 3.25)
project(OpenMP_TD)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_COMPILER "/usr/local/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")
include_directories("/usr/local/Cellar/libomp/16.0.4/include")
link_directories("/usr/local/Cellar/libomp/16.0.4/lib")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp -L/usr/local/opt/llvm/lib")

add_executable(OpenMP_TD main.cpp)
target_link_libraries(OpenMP_TD omp)
