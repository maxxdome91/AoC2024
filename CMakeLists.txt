cmake_minimum_required(VERSION 3.30)
project(AoC2024)

set(CMAKE_CXX_STANDARD 20)

add_executable(AoC2024 main.cpp
        src/util.cpp
        src/util.h
        src/source.cpp
        src/source.h
)
