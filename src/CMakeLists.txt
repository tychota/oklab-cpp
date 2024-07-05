# Define a library target named 'oklab'
add_library(oklab SHARED
    OkLxx.cpp
    RGB.cpp
    P3.cpp
    ColorConversionsInternals.cpp
    ColorConversions.cpp
)

# Add definitions based on the selected mapping algorithm
if(MAPPING_ALGORITHM STREQUAL "CSS4")
    target_compile_definitions(oklab PRIVATE MAPPING_CSS4)
elseif(MAPPING_ALGORITHM STREQUAL "CLAMP")
    target_compile_definitions(oklab PRIVATE MAPPING_CLAMP)
endif()

# target_compile_definitions(oklab PRIVATE DEBUG_LOOP_COUNT)

# Specify include directories for this target
target_include_directories(oklab PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(oklab PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)