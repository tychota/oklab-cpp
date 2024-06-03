#pragma once

#include <array>
#include <cmath>
#include <algorithm>

using Matrix3x3 = std::array<std::array<double, 3>, 3>;
using Vector3 = std::array<double, 3>;

// Matrix-vector multiplication
Vector3 multiply(const Matrix3x3 &matrix, const Vector3 &vec);

// Elementwise cube root
Vector3 elementwiseCbrt(const Vector3 &vec);

// Elementwise power
Vector3 elementwisePow(const Vector3 &vec, double exponent);

// Scalar division
Vector3 scalarDiv(const Vector3 &vec, double scalar);

// Scalar multiplication
Vector3 scalarMul(const Vector3 &vec, double scalar);

// Scalar addition
Vector3 scalarAdd(const Vector3 &vec, double scalar);