#pragma once

#include <array>
#include <type_traits>

// Tag definitions
struct RGBTag
{
};
struct P3Tag
{
};
struct GammaSRGBTag
{
};
struct LinearSRGBTag
{
};
struct GammaP3Tag
{
};
struct LinearP3Tag
{
};
struct LMSTag
{
};
struct OklabTag
{
};
struct OklchTag
{
};

// Tagged array template
template <typename T, int N, typename Tag>
struct TaggedArray : public std::array<T, N>
{
    using Base = std::array<T, N>;
    using Base::Base; // Inherit constructors

    // Default constructor
    TaggedArray() : Base() {}

    // Initializer list constructor
    TaggedArray(std::initializer_list<T> ilist)
    {
        std::copy_n(ilist.begin(), N, this->begin());
    }

    // Allow conversion from std::array
    TaggedArray(const std::array<T, N> &other) : Base(other) {}

    // Copy constructor
    TaggedArray(const TaggedArray &other) : Base(other) {}

    // Move constructor
    TaggedArray(TaggedArray &&other) noexcept : Base(std::move(other)) {}

    // Assignment from std::array
    TaggedArray &operator=(const std::array<T, N> &other)
    {
        Base::operator=(other);
        return *this;
    }

    // Copy assignment operator
    TaggedArray &operator=(const TaggedArray &other)
    {
        Base::operator=(other);
        return *this;
    }

    // Move assignment operator
    TaggedArray &operator=(TaggedArray &&other) noexcept
    {
        Base::operator=(std::move(other));
        return *this;
    }
};

/**
 * @file ColorTypes.h
 * @brief Contains definitions for color types used throughout the color processing library.
 */

namespace oklab
{

    /**
     * @brief Represents an RGB color using integers (0-255).
     */
    using RGB = TaggedArray<int, 3, RGBTag>;

    /**
     * @brief Represents an RGB color in gamma-scaled doubleing-point format.
     */
    using GammaSRGB = TaggedArray<double, 3, GammaSRGBTag>;

    /**
     * @brief Represents an RGB color in linear-scaled doubleing-point format.
     */
    using LinearSRGB = TaggedArray<double, 3, LinearSRGBTag>;

    /**
     * @brief Represents an P3 color using integers (0-255).
     */
    using P3 = TaggedArray<int, 3, P3Tag>;

    /**
     * @brief Represents an P3 color in gamma-scaled doubleing-point format.
     */
    using GammaP3 = TaggedArray<double, 3, GammaP3Tag>;

    /**
     * @brief Represents an P3 color in linear-scaled doubleing-point format.
     */
    using LinearP3 = TaggedArray<double, 3, LinearP3Tag>;

    /**
     * @brief Represents an intermediate LMS color.
     */
    using LMS = TaggedArray<double, 3, LMSTag>;

    /**
     * @brief Represents a color in the Oklab color space.
     */
    using Oklab = TaggedArray<double, 3, OklabTag>;

    /**
     * @brief Represents a color in the Oklch color space.
     */
    using Oklch = TaggedArray<double, 3, OklchTag>;

} // namespace oklab