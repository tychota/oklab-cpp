# Find the cppbenchmark package
find_package(cppbenchmark REQUIRED)

# Define the benchmark executable
add_executable(oklab_benchmark
    OklabBenchmark.cpp
)

# Link with the library and benchmark
target_link_libraries(oklab_benchmark PRIVATE oklab cppbenchmark::cppbenchmark)