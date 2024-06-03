cmake_minimum_required(VERSION 3.29.3)
project(oklab CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")

add_library(Oklab STATIC src/Oklab.cpp src/MatrixOperations.cpp)

add_executable(oklabMain src/main.cpp)
target_link_libraries(oklabMain Oklab)

find_package(GTest REQUIRED)
enable_testing()
add_executable(oklabTests tests/OklabTests.cpp)
target_link_libraries(oklabTests Oklab gtest::gtest)

find_package(benchmark REQUIRED)
add_executable(oklabBenchmark benchmarks/OklabBenchmark.cpp)
target_link_libraries(oklabBenchmark Oklab benchmark::benchmark_main)