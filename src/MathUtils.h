#pragma once

#include <cmath>
#include <array>

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
    double cbrt(double value);

    /**
     * @brief Constrains an angle to the range [0, 360).
     *
     * This function constrains an angle to the range [0, 360) by taking the modulo 360 of the input value.
     *
     * @param angle The input angle.
     * @return The angle constrained to the range [0, 360).
     */
    double constrainAngle(double angle);

    /**
     * @brief Multiplies a 3x3 matrix by a 3D vector.
     *
     * This function multiplies a 3x3 matrix by a 3D vector, returning the resulting vector.
     *
     * @param matrix The 3x3 matrix to multiply.
     * @param vector The 3D vector to multiply.
     */
    std::array<double, 3> multiplyMatrix(const double matrix[3][3], const std::array<double, 3> &vector);
} // namespace oklab