#include "ColorConversions.h"
#include "ColorConversionsInternal.h"

#include "gamutMapping/CSS4.h"

namespace oklab
{
    P3 rgbToP3(const RGB &rgb)
    {
        Oklab oklab = rgbToOklab(rgb);
        return oklabToP3(oklab);
    }

    RGB p3ToRgb(const P3 &p3)
    {
        Oklab oklab = p3ToOklab(p3);
        return oklabToRgb(oklab);
    }
}