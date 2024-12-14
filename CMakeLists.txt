cmake_minimum_required(VERSION 3.30)
project(CustomBench)

set(CMAKE_CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
        include/ram_bench.h
        src/ram_bench.cpp
        include/cpu_bench.h
        src/cpu_bench.cpp
)

add_executable(CustomBench ${SOURCES})
