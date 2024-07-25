#include "OkLxx.h"
#include <cmath>

#include "MathUtils.h"

/**
 * @file ColorUtils.h
 * @brief Provides utility functions for color conversions between linear and gamma-encoded values.
 */

const double PI = M_PI;

const double LMSG_TO_OKLAB[3][3] = {
    {0.2104542683093140, 0.7936177747023054, -0.0040720430116193},
    {1.9779985324311684, -2.4285922420485799, 0.4505937096174110},
    {0.0259040424655478, 0.7827717124575296, -0.8086757549230774}};

const double OKLAB_TO_LMSG[3][3] = {
    {1.0000000000000000, 0.3963377773761749, 0.2158037573099136},
    {1.0000000000000000, -0.1055613458156586, -0.0638541728258133},
    {1.0000000000000000, -0.0894841775298119, -1.2914855480194092}};

namespace oklab
{
    Oklab oklchToOklab(const Oklch &oklch)
    {
        if (std::isnan(oklch[2]))
        {
            return Oklab{oklch[0], 0, 0};
        }
        else
        {
            double angle = constrainAngle(oklch[2]) * PI / 180.0;

            return Oklab{
                oklch[0],
                std::cos(angle) * oklch[1],
                std::sin(angle) * oklch[1]};
        }
    }

    Oklch oklabToOklch(const Oklab &oklab)
    {
        double epsilon = 0.0002;
        if (std::abs(oklab[1]) < epsilon && std::abs(oklab[2]) < epsilon)
        {
            return Oklch{oklab[0], 0, NAN};
        }
        else
        {
            return Oklch{
                oklab[0],
                std::sqrt(oklab[1] * oklab[1] + oklab[2] * oklab[2]),
                constrainAngle(std::atan2(oklab[2], oklab[1]) * 180.0 / PI)};
        }
    }

    Oklab lmsToOklab(const LMS &lms)
    {
        std::array<double, 3> lms_g = {cbrt(lms[0]), cbrt(lms[1]), cbrt(lms[2])};
        Oklab oklab = multiplyMatrix(LMSG_TO_OKLAB, lms_g);
        return oklab;
    }

    LMS oklabToLms(const Oklab &oklab)
    {
        std::array<double, 3> lms_g = multiplyMatrix(OKLAB_TO_LMSG, oklab);
        LMS lms = {lms_g[0] * lms_g[0] * lms_g[0], lms_g[1] * lms_g[1] * lms_g[1], lms_g[2] * lms_g[2] * lms_g[2]};
        return lms;
    }

    double deltaE(const Oklab &oklab_1, const Oklab &oklab_2)
    {
        double l1 = oklab_1[0];
        double a1 = oklab_1[1];
        double b1 = oklab_1[2];

        double l2 = oklab_2[0];
        double a2 = oklab_2[1];
        double b2 = oklab_2[2];

        double deltaL = l1 - l2;
        // TODO: 2 is debated here: https://github.com/w3c/csswg-drafts/pull/10063
        // However, reference implementation use 1 so we do too.
        const double FACTOR = 1;
        double deltaA = FACTOR * (a1 - a2);
        double deltaB = FACTOR * (b1 - b2);

        double deltaE = std::sqrt(deltaL * deltaL + deltaA * deltaA + deltaB * deltaB);

        return deltaE;
    }
} // namespace oklab