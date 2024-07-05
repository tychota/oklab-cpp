#pragma once

#include "ColorTypes.h"
#include "ColorConversions.h" // For conversion functions

/**
 * @file ColorManipulations.h
 * @brief Provides template functions for color manipulations performed in the Oklab/Oklch color spaces.
 */

namespace oklab
{

    /**
     * @brief Interpolates between two colors in the Oklab color space.
     * @param c1 First color for interpolation.
     * @param c2 Second color for interpolation.
     * @param t Interpolation parameter [0, 1].
     * @return Interpolated color of the same type as the input.
     */
    template <typename ColorType>
    ColorType interpolateColor(const ColorType &c1, const ColorType &c2, double t)
    {
        // Convert colors to Oklab
        Oklab oklab1 = convertToOklab(c1);
        Oklab oklab2 = convertToOklab(c2);

        // Perform interpolation in Oklab
        Oklab resultOklab;
        for (size_t i = 0; i < oklab1.size(); ++i)
        {
            resultOklab[i] = oklab1[i] * (1 - t) + oklab2[i] * t;
        }

        // Convert back to original color space with gamut mapping
        return convertFromOklab<ColorType>(resultOklab);
    }

    /**
     * @brief Darkens a given color by reducing its lightness in the Oklch color space.
     * @param color Color to darken.
     * @param amount Amount to reduce the lightness (0 - no change, 1 - fully darkened).
     * @return Darkened color of the same type as the input.
     */
    template <typename ColorType>
    ColorType darken(const ColorType &color, double amount)
    {
        Oklch oklch = convertToOklch(color);
        oklch[0] *= (1 - amount); // Adjust lightness channel

        return convertFromOklch<Colorype>(oklch);
    }

    /**
     * @brief Lightens a given color by increasing its lightness in the Oklch color space.
     * @param color Color to lighten.
     * @param amount Amount to increase the lightness (0 - no change, 1 - fully lightened).
     * @return Lightened color of the same type as the input.
     */
    template <typename ColorType>
    ColorType lighten(const ColorType &color, double amount)
    {
        Oklch oklch = convertToOklch(color);
        oklch[0] = std::min(1.0, oklch[0] + (1 - oklch[0]) * amount); // Ensure lightness does not exceed bounds

        return convertFromOklch<ColorType>(oklch);
    }

} // namespace oklab