#include "gtest/gtest.h"
#include "../src/ColorConversionsInternal.h"

using namespace oklab;

TEST(p3ToRGB__NoGammut, Black)
{
    P3 input{0, 0, 0};
    RGB expected{0, 0, 0};

    RGB result = convertP3ToRgbInternal(input, false);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_NoGammut, FullRed)
{
    P3 input{255, 0, 0};
    RGB expected{279, -58, -38};
    RGB result = convertP3ToRgbInternal(input, false);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_NoGammut, FullGreen)
{
    P3 input{0, 255, 0};
    RGB expected{-130, 260, -79};
    RGB result = convertP3ToRgbInternal(input, false);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_NoGammut, FullBlue)
{
    P3 input{0, 0, 255};
    RGB expected{0, 0, 266};
    RGB result = convertP3ToRgbInternal(input, false);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_NoGammut, Yellow)
{
    P3 input{255, 255, 0};
    RGB expected{255, 255, -88};
    RGB result = convertP3ToRgbInternal(input, false);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_NoGammut, Magenta)
{
    P3 input{255, 0, 255};
    RGB expected{279, -58, 264};
    RGB result = convertP3ToRgbInternal(input, false);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_NoGammut, Cyan)
{
    P3 input{0, 255, 255};
    RGB expected{-130, 260, 257};
    RGB result = convertP3ToRgbInternal(input, false);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_NoGammut, White)
{
    P3 input{255, 255, 255};
    RGB expected{255, 255, 255};
    RGB result = convertP3ToRgbInternal(input, false);
    EXPECT_EQ(result, expected);
}

// With Gamut

TEST(p3ToRGB_WithGamut, Black)
{
    P3 input{0, 0, 0};
    RGB expected{0, 0, 0};
    RGB result = convertP3ToRgbInternal(input, true);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_WithGamut, FullRed)
{
    P3 input{255, 0, 0};
    RGB expected{255, 11, 12};
    RGB result = convertP3ToRgbInternal(input, true);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_WithGamut, FullGreen)
{
    P3 input{0, 255, 0};
    RGB expected{0, 251, 41};
    RGB result = convertP3ToRgbInternal(input, true);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_WithGamut, FullBlue)
{
    P3 input{0, 0, 255};
    RGB expected{0, 0, 255};
    RGB result = convertP3ToRgbInternal(input, true);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_WithGamut, Yellow)
{
    P3 input{255, 255, 0};
    RGB expected{254, 255, 0};
    RGB result = convertP3ToRgbInternal(input, true);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_WithGamut, Magenta)
{
    P3 input{255, 0, 255};
    RGB expected{255, 43, 251};
    RGB result = convertP3ToRgbInternal(input, true);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_WithGamut, Cyan)
{
    P3 input{0, 255, 255};
    RGB expected{0, 254, 251};
    RGB result = convertP3ToRgbInternal(input, true);
    EXPECT_EQ(result, expected);
}

TEST(p3ToRGB_WithGamut, White)
{
    P3 input{255, 255, 255};
    RGB expected{255, 255, 255};
    RGB result = convertP3ToRgbInternal(input, true);
    EXPECT_EQ(result, expected);
}