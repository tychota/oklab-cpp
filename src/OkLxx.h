#pragma once

#include "ColorTypes.h"

namespace oklab
{
    /**
     * @brief Converts a color from Oklch to Oklab color space.
     *
     * Oklch is a cylindrical representation of the Oklab color space,
     * where L is lightness, C is chroma, and H is hue.
     *
     * @param color The color in Oklch space (L, C, H).
     * @return The corresponding color in Oklab space (L, a, b).
     */
    Oklab oklchToOklab(const Oklch &color);

    /**
     * @brief Converts a color from Oklab to Oklch color space.
     *
     * Oklab is a perceptually uniform color space,
     * where L is lightness, a is green-red, and b is blue-yellow.
     *
     * @param color The color in Oklab space (L, a, b).
     * @return The corresponding color in Oklch space (L, C, H).
     */
    Oklch oklabToOklch(const Oklab &color);

    /**
     * @brief Converts a color from LMS to Oklab color space.
     *
     * LMS is a color space based on the response of the three types of cones in the human eye.
     *
     * @param lms The color in LMS space.
     * @return The corresponding color in Oklab space.
     */
    Oklab lmsToOklab(const LMS &lms);

    /**
     * @brief Converts a color from Oklab to LMS color space.
     *
     * @param oklab The color in Oklab space.
     * @return The corresponding color in LMS space.
     */
    LMS oklabToLms(const Oklab &Oklab);

    /**
     * @brief Calculates the perceptual difference between two colors in Oklab space.
     * 
     * This function implements the ΔE (Delta E) color difference metric for Oklab.
     * A lower value indicates colors that are perceptually closer.
     * 
     * @param color1 The first color in Oklab space.
     * @param color2 The second color in Oklab space.
     * @return The perceptual difference between the two colors.
     */
    double deltaE(const Oklab &color1, const Oklab &color2);
}