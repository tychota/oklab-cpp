# Define the example executable
add_executable(example_usage
    main.cpp
)

find_package(args REQUIRED)

# Link with the oklab library
target_link_libraries(example_usage PRIVATE oklab taywee::args)