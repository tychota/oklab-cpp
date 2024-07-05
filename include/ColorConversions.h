#pragma once

#include "ColorTypes.h"
#include <algorithm> // Include if needed for std::min, std::max, etc.

/**
 * @file ColorConversions.h
 * @brief Provides functions for converting between different color spaces.
 */

namespace oklab
{

    /**
     * @brief Converts an RGB color to an Oklab color.
     * @param color RGB color to convert.
     * @return Oklab representation of the input color.
     */
    Oklab rgbToOklab(const RGB &color);

    /**
     * @brief Converts an Oklab color to an RGB color.
     * @param color Oklab color to convert.
     * @return RGB representation of the input color.
     * Includes gamut mapping to ensure the output stays within the valid RGB range.
     */
    RGB oklabToRgb(const Oklab &color);

    /**
     * @brief Converts a P3 color to an Oklab color.
     * @param color P3 color to convert.
     * @return Oklab representation of the input color.
     */
    Oklab p3ToOklab(const P3 &color);

    /**
     * @brief Converts an Oklab color to a P3 color.
     * @param color Oklab color to convert.
     * @return P3 representation of the input color.
     * Includes gamut mapping to ensure the output stays within the valid P3 range.
     */
    P3 oklabToP3(const Oklab &color);

    /**
     * @brief Converts a generic color type to Oklab.
     * This template function must be specialized for each supported color type.
     * @param color Color to convert.
     * @return Oklab representation of the input color.
     */
    template <typename ColorType>
    Oklab convertToOklab(const ColorType &color);

    /**
     * @brief Converts Oklab to a generic color type.
     * This template function must be specialized for each supported color type,
     * including gamut mapping.
     * @param oklab Oklab color to convert.
     * @return Color representation of the input Oklab.
     */
    template <typename ColorType>
    ColorType convertFromOklab(const Oklab &oklab);

    /**
     * @brief Converts a RGB color to P3.
     *
     * @param color RGB color to convert.
     * @return P3 representation of the input color.
     */
    P3 rgbToP3(const RGB &color);

    /**
     * @brief Converts an P3 color to RGB.
     *
     * @param color P3 color to convert.
     * @return RGB representation of the input color.
     */
    RGB p3ToRgb(const P3 &color);
} // namespace oklab