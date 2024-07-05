#pragma once

#include "gtest/gtest.h"
#include <sstream>
#include <type_traits>
#include <iostream>

// Generalized helper function to convert any sequence container to a string for printing
template <typename Container>
std::string containerToString(const Container &container)
{
    std::ostringstream oss;
    oss << "{";
    for (size_t i = 0; i < container.size(); ++i)
    {
        oss << container[i];
        if (i < container.size() - 1)
            oss << ", ";
    }
    oss << "}";
    return oss.str();
}

// Enhanced macro to check if two sequence containers are approximately equal and print full contents if they are not
#define EXPECT_VECTOR_EQ(vec1, vec2, tol)                                                                                                 \
    do                                                                                                                                    \
    {                                                                                                                                     \
        ASSERT_EQ(vec1.size(), vec2.size()) << "Containers are of unequal length";                                                        \
        bool pass = true;                                                                                                                 \
        for (size_t i = 0; i < vec1.size(); ++i)                                                                                          \
        {                                                                                                                                 \
            if (std::abs(vec1[i] - vec2[i]) > tol)                                                                                        \
            {                                                                                                                             \
                pass = false;                                                                                                             \
                ADD_FAILURE_AT(__FILE__, __LINE__) << "Containers differ at index " << i                                                  \
                                                   << ": " << vec1[i] << " (actual) vs " << vec2[i] << " (expected), tolerance: " << tol; \
            }                                                                                                                             \
        }                                                                                                                                 \
        if (!pass)                                                                                                                        \
        {                                                                                                                                 \
            ADD_FAILURE() << "Full Containers:\nActual:   " << containerToString(vec1)                                                    \
                          << "\nExpected: " << containerToString(vec2);                                                                   \
        }                                                                                                                                 \
    } while (0)