#pragma once

#include "ColorTypes.h"

namespace oklab
{
    RGB convertP3ToRgbInternal(const P3 &p3, bool applyGamutCorrection);
}