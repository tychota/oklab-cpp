#pragma once

#include <array>
#include "MatrixOperations.h"

using RGB = std::array<int, 3>;
using GsRGB = std::array<double, 3>;
using LsRGB = std::array<double, 3>;
using Oklab = std::array<double, 3>;
using Oklch = std::array<double, 3>;

RGB scaleTo255(const GsRGB &color);
GsRGB scaleTo01(const RGB &color);
bool inGamut(const GsRGB &color);
GsRGB clip(const GsRGB &color);
GsRGB srgbGammaToLinear(const GsRGB &color);
GsRGB srgbLinearToGamma(const GsRGB &color);
Oklab linearSrgbToOklab(const LsRGB &color);
LsRGB oklabToLinearSrgb(const Oklab &color);
Oklab oklchToOklab(const Oklch &color);
Oklch oklabToOklch(const Oklab &color);
double delta(const Oklab &c1, const Oklab &c2);
RGB interpolateColor(const RGB &c1, const RGB &c2, double t);