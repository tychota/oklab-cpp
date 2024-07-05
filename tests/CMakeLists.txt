# Find package for GoogleTest
find_package(GTest REQUIRED)

# Define the test executable
add_executable(oklab_tests
    p3ToRgbVectorsTests.cpp
    validRoundTripsTests.cpp
)

# Link with the library and GoogleTest
target_link_libraries(oklab_tests PRIVATE oklab GTest::GTest GTest::Main)

# Include GoogleTest in testing
include(GoogleTest)
gtest_discover_tests(oklab_tests)