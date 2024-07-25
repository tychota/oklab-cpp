#include "ColorConversions.h"
#include "ColorConversionsInternal.h"

#include "gamutMapping/CSS4.h"

namespace oklab
{
    // NOTE: this internal function is use so we can test:
    // - a simple P3 to RGB conversion with applyGamutCorrection = false
    // - the gamut mapping algorithm with applyGamutCorrection = true
    // It is not meant to be used outside of the file.
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