#include "MathUtils.h"

#include <cmath>

namespace oklab
{
    double cbrt(double value)
    {
        double sign = value < 0 ? -1.0 : 1.0;
        return sign * std::cbrt(std::abs(value));
    }

    double constrainAngle(double angle)
    {
        return std::fmod((std::fmod(angle, 360.0) + 360.0), 360.0);
    }

    std::array<double, 3> multiplyMatrix(const double matrix[3][3], const std::array<double, 3> &vector)
    {
        std::array<double, 3> result = {0.0, 0.0, 0.0};
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                result[i] += matrix[i][j] * vector[j];
            }
        }
        return result;
    }

} // namespace oklab