#include "ColorConversions.h"
#include "ColorConversionsInternal.h"

#include <cmath>
#include <algorithm>

#include "OkLxx.h"
#include "MathUtils.h"
#include "ColorUtils.h"
#include "gamutMapping/CSS4.h"

// Define the transformation matrices as constants
const double RGB_TO_LMS[3][3] = {
    {0.4122214694707629, 0.5363325372617349, 0.0514459932675022},
    {0.2119034958178251, 0.6806995506452345, 0.1073969535369406},
    {0.0883024591900564, 0.2817188391361215, 0.6299787016738222}};

const double LMS_TO_RGB[3][3] = {
    {4.0767416360759601, -3.3077115392580625, 0.2309699031821046},
    {-1.2684379732850317, 2.6097573492876882, -0.3413193760026572},
    {-0.0041960761386754, -0.7034186179359361, 1.7076146940746113}};

namespace oklab
{
    // Template specialization for RGB
    template <>
    Oklab convertToOklab<RGB>(const RGB &rgb)
    {
        return rgbToOklab(rgb);
    }

    template <>
    RGB convertFromOklab<RGB>(const Oklab &oklab)
    {
        return oklabToRgb(oklab);
    }

    template <>
    LinearSRGB clipToGamut<LinearSRGB>(const LinearSRGB &linearRgb)
    {
        return LinearSRGB{
            std::clamp(linearRgb[0], 0.0, 1.0),
            std::clamp(linearRgb[1], 0.0, 1.0),
            std::clamp(linearRgb[2], 0.0, 1.0)};
    };

    inline LinearSRGB rgbToLinearRgb(const RGB &rgb)
    {
        double r = gammaToLinear(rgb[0] / 255.0);
        double g = gammaToLinear(rgb[1] / 255.0);
        double b = gammaToLinear(rgb[2] / 255.0);

        return {r, g, b};
    }

    inline RGB linearRgbToRgb(const LinearSRGB &linearRgb)
    {
        int r = static_cast<int>(std::round(linearToGamma(linearRgb[0]) * 255.0));
        int g = static_cast<int>(std::round(linearToGamma(linearRgb[1]) * 255.0));
        int b = static_cast<int>(std::round(linearToGamma(linearRgb[2]) * 255.0));

        return RGB{r, g, b};
    }

    inline Oklab linearRgbToOklab(const LinearSRGB &linearRgb)
    {
        LMS lms = multiplyMatrix(RGB_TO_LMS, linearRgb);
        Oklab oklab = lmsToOklab(lms);
        return oklab;
    }

    Oklab rgbToOklab(const RGB &rgb)
    {
        double r = gammaToLinear(rgb[0] / 255.0);
        double g = gammaToLinear(rgb[1] / 255.0);
        double b = gammaToLinear(rgb[2] / 255.0);

        return linearRgbToOklab({r, g, b});
    }

    inline LinearSRGB oklabToLinearRgb(const Oklab &oklab)
    {
        LMS lms = oklabToLms(oklab);
        LinearSRGB rgb = multiplyMatrix(LMS_TO_RGB, lms);
        return rgb;
    }

    template <>
    bool isInGamut<LinearSRGB>(const LinearSRGB &linearRgb)
    {
        return linearRgb[0] >= 0.0 && linearRgb[0] <= 1.0 &&
               linearRgb[1] >= 0.0 && linearRgb[1] <= 1.0 &&
               linearRgb[2] >= 0.0 && linearRgb[2] <= 1.0;
    }

    template <>
    LinearSRGB oklabToLinearColor<LinearSRGB>(const Oklab &oklab)
    {
        return oklabToLinearRgb(oklab);
    }

    template <>
    RGB linearColorToColor<LinearSRGB, RGB>(const LinearSRGB &linearRgb)
    {
        return linearRgbToRgb(linearRgb);
    }

    template <>
    Oklab linearColorToOklab<LinearSRGB>(const LinearSRGB &linearRgb)
    {
        return linearRgbToOklab(linearRgb);
    }

    RGB oklabToRgb(const Oklab &oklab)
    {
#ifdef MAPPING_CSS4
        return performCssGamutMapping<RGB, LinearSRGB>(oklab);
#elif MAPPING_CLAMP
        LinearSRGB linearRgb = oklabToLinearRgb(oklab);
        LinearSRGB clippedLinearRgb = clipToGamut<LinearSRGB>(linearRgb);
        return linearColorToColor<LinearSRGB, RGB>(clippedLinearRgb);
#else
        return performCssGamutMapping<RGB, LinearSRGB>(oklab);
#endif
    }
}