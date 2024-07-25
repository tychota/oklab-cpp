#include <vector>
#include "gtest/gtest.h"
#include "ColorConversions.h"

#include "CustomMatchers.h"

using namespace oklab;

std::vector<P3> testP3Colors = {
    {0, 0, 0},
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
    {255, 255, 255},
    {128, 128, 128}};

std::vector<RGB> testRGBColors = {
    //{0, 0, 0},
    //{255, 0, 0},
    {0, 255, 0},
    //{0, 0, 255},
    //{255, 255, 255},
    //{128, 128, 128}
};

TEST(ValidRoundTrips, P3ToOklabToP3)
{
    for (const auto &original : testP3Colors)
    {
        Oklab oklab = p3ToOklab(original);
        P3 backToP3 = oklabToP3(oklab);

        EXPECT_VECTOR_EQ(original, backToP3, 0);
    }
}

TEST(ValidRoundTrips, RgbToOklabToRgb)
{
    for (const auto &original : testRGBColors)
    {
        Oklab oklab = rgbToOklab(original);
        RGB backToRgb = oklabToRgb(oklab);
        EXPECT_VECTOR_EQ(original, backToRgb, 0);
    }
}

TEST(ValidRoundTrips, RgbToP3ToRgb)
{
    for (const auto &original : testRGBColors)
    {
        P3 p3 = rgbToP3(original);
        RGB backToRgb = p3ToRgb(p3);
        EXPECT_VECTOR_EQ(original, backToRgb, 5);
    }
}