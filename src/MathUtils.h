#pragma once

#include <cmath>

/**
 * @file MathUtils.h
 * @brief Provides utility functions for mathematical operations.
 */

namespace oklab
{
    /**
     * @brief Computes the cube root of a doubleing point number, retaining the sign.
     *
     * This function computes the cube root of a doubleing point number, preserving the sign of the input value.
     *
     * @param value The input value.
     * @return The cube root of the input value.
     */
    inline double cbrt(double value)
    {
        double sign = value < 0 ? -1.0 : 1.0;
        return sign * std::cbrt(std::abs(value));
    }

    /**
     * @brief Constrains an angle to the range [0, 360).
     *
     * This function constrains an angle to the range [0, 360) by taking the modulo 360 of the input value.
     *
     * @param angle The input angle.
     * @return The angle constrained to the range [0, 360).
     */
    inline double constrainAngle(double angle)
    {
        return std::fmod((std::fmod(angle, 360.0) + 360.0), 360.0);
    }

    /**
     * @brief Multiplies a 3x3 matrix by a 3D vector.
     *
     * This function multiplies a 3x3 matrix by a 3D vector, returning the resulting vector.
     *
     * @param matrix The 3x3 matrix to multiply.
     * @param vector The 3D vector to multiply.
     */
    inline std::array<double, 3> multiplyMatrix(const double matrix[3][3], const std::array<double, 3> &vector)
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