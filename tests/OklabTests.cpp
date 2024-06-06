#include "gtest/gtest.h"
#include "../src/Oklab.h"

// TODO: write tests for the Oklab functions

TEST(ColorTest, GamutMapping)
{
    RGB color1 = {255, 0, 0}; // Red
    RGB color2 = {0, 0, 255}; // Blue

    float t = 0.5;
    RGB result = interpolateColor(color1, color2, t);

    EXPECT_GE(result[0], 0);
    EXPECT_LE(result[0], 255);
    EXPECT_GE(result[1], 0);
    EXPECT_LE(result[1], 255);
    EXPECT_GE(result[2], 0);
    EXPECT_LE(result[2], 255);
}

TEST(ColorTest, NegativeInterpolation)
{
    RGB color1 = {255, 0, 0}; // Red
    RGB color2 = {0, 0, 255}; // Blue

    float t = -0.5;
    RGB result = interpolateColor(color1, color2, t);

    // Ensure the result is clamped to valid RGB values
    EXPECT_GE(result[0], 0);
    EXPECT_LE(result[0], 255);
    EXPECT_GE(result[1], 0);
    EXPECT_LE(result[1], 255);
    EXPECT_GE(result[2], 0);
    EXPECT_LE(result[2], 255);
}
