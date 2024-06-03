#include "MatrixOperations.h"

Vector3 multiply(const Matrix3x3 &matrix, const Vector3 &vec)
{
    Vector3 result = {0, 0, 0};
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}

Vector3 elementwiseCbrt(const Vector3 &vec)
{
    Vector3 result;
    std::transform(vec.begin(), vec.end(), result.begin(), [](double v)
                   { return std::copysign(std::pow(std::abs(v), 1.0 / 3.0), v); });
    return result;
}

Vector3 elementwisePow(const Vector3 &vec, double exponent)
{
    Vector3 result;
    std::transform(vec.begin(), vec.end(), result.begin(), [exponent](double v)
                   { return std::pow(v, exponent); });
    return result;
}

Vector3 scalarDiv(const Vector3 &vec, double scalar)
{
    Vector3 result;
    std::transform(vec.begin(), vec.end(), result.begin(), [scalar](double v)
                   { return v / scalar; });
    return result;
}

Vector3 scalarMul(const Vector3 &vec, double scalar)
{
    Vector3 result;
    std::transform(vec.begin(), vec.end(), result.begin(), [scalar](double v)
                   { return v * scalar; });
    return result;
}

Vector3 scalarAdd(const Vector3 &vec, double scalar)
{
    Vector3 result;
    std::transform(vec.begin(), vec.end(), result.begin(), [scalar](double v)
                   { return v + scalar; });
    return result;
}
