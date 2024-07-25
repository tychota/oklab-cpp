#include <cmath>

#include "ColorTypes.h"
#include "ColorUtils.h"

/**
 * @file ColorUtils.h
 * @brief Provides utility functions for color conversions between linear and gamma-encoded values.
 */

namespace oklab
{
    // Converts a linear light value to a gamma-encoded value.
    double gammaToLinear(double value)
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

    // NOTE: We may optimize this by doing a branchless version and precomputing the constants.
    // eg:
    // double gammaToLinear(double value) {
    //     constexpr double a = 0.055;
    //     constexpr double gamma = 2.4;
    //     constexpr double threshold = 0.04045;
    //     constexpr double scale = 1.0 / 12.92;
    //     constexpr double scale2 = 1.0 / 1.055;

    //     double absValue = std::abs(value);
    //     double result = (absValue <= threshold)
    //                     ? absValue * scale
    //                     : std::pow((absValue + a) * scale2, gamma);
    //     return std::copysign(result, value);
    // }

    // Converts a gamma-encoded light value to a linear value.
    double linearToGamma(double value)
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
} // namespace oklab