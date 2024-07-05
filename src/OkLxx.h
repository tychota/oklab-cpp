#pragma once

#include "ColorTypes.h"

namespace oklab
{
    Oklab oklchToOklab(const Oklch &color);
    Oklch oklabToOklch(const Oklab &color);

    Oklab lmsToOklab(const LMS &lms);
    LMS oklabToLms(const Oklab &Oklab);

    double deltaE(const Oklab &color1, const Oklab &color2);
}