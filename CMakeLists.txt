cmake_minimum_required(VERSION 3.14)
project(Cache_iLab)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -I ../include")

add_executable(Cache_iLab source/main.cpp include/cache.hpp source/cache.cpp)