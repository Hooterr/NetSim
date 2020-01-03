cmake_minimum_required(VERSION 3.13)
project(src)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_executable(src main.cpp include/package.hpp include/types.hpp include/storage_types.hpp package.cpp storage_types.cpp)