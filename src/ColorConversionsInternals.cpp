#include "ColorConversions.h"
#include "ColorConversionsInternal.h"

#include "gamutMapping/CSS4.h"

namespace oklab
{
    RGB convertP3ToRgbInternal(const P3 &p3, bool applyGamutCorrection)
    {
        Oklab oklab = p3ToOklab(p3);

        if (applyGamutCorrection)
        {
            return performCssGamutMapping<RGB, LinearSRGB>(oklab);
        }
        else
        {
            LinearSRGB linearRgb = oklabToLinearColor<LinearSRGB>(oklab);
            return linearColorToColor<LinearSRGB, RGB>(linearRgb);
        }
    }
}