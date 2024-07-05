cmake_minimum_required(VERSION 3.29.3)
project(oklab CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories for the entire project
include_directories(${PROJECT_SOURCE_DIR}/include)

# Mapping algorithm selection
set(MAPPING_ALGORITHM "CSS4" CACHE STRING "Choose the mapping algorithm: CSS4, CLAMP")
set_property(CACHE MAPPING_ALGORITHM PROPERTY STRINGS "CSS4" "CLAMP")

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)
add_subdirectory(examples)

enable_testing()  # Enables integration of test frameworks