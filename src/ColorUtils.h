#pragma once
#include <cmath>

#include "ColorTypes.h"
#include "ColorUtils.h"

/**
 * @file ColorUtils.h
 * @brief Provides utility functions for color conversions between linear and gamma-encoded values.
 */

namespace oklab
{
    /**
     * @brief Converts a gamma-encoded value to a linear light value.
     *
     * This function applies the inverse of the sRGB transfer function to a gamma-encoded value,
     * converting it from a non-linear space used for display purposes to a linear space suitable
     * for arithmetic operations. This conversion is essential for accurate color processing tasks.
     *
     * @param value The gamma-encoded value, typically in the range [0, 1].
     * @return The linearized value.
     */
    inline double gammaToLinear(double value)
    {
        double absValue = std::abs(value);

        if (absValue <= 0.04045)
        {
            return value / 12.92;
        }
        else
        {
            return std::copysign(std::pow((absValue + 0.055) / 1.055, 2.4), value);
        }
    }

    /**
     * @brief Converts a linear light value to a gamma-encoded value.
     *
     * This function applies the sRGB transfer function to a linear light value, converting it from
     * a linear space where arithmetic operations are accurate to a non-linear gamma-encoded space
     * suitable for storage and display. This encoding compensates for the non-linear response of
     * human vision to light and is standard for many display systems.
     *
     * @param value The linear light value, typically in the range [0, 1].
     * @return The gamma-encoded value.
     */
    inline double linearToGamma(double value)
    {
        double absValue = std::abs(value);

        if (absValue <= 0.0031308)
        {
            return 12.92 * value;
        }
        else
        {
            return std::copysign(1.055 * std::pow(absValue, 1.0 / 2.4) - 0.055, value);
        }
    }

    /**
     * @brief Clip a color to its gamut.
     * This template function must be specialized for each supported color type.
     * @param color Color to convert.
     * @return Clipped color.
     */
    template <typename ColorType>
    ColorType clipToGamut(const ColorType &color);
} // namespace oklab