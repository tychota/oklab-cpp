#include "ColorConversions.h"

#include <cmath>
#include <algorithm>

#include "OkLxx.h"
#include "MathUtils.h"
#include "ColorUtils.h"
#include "gamutMapping/CSS4.h"

// Define the transformation matrices as constants
const double P3_TO_LMS[3][3] = {
    {0.4813798527499544, 0.4621183710113182, 0.0565017762387275},
    {0.2288319418112447, 0.6532168193835679, 0.1179512388051878},
    {0.0839457523229932, 0.2241652709775665, 0.6918889766994405}};

const double LMS_TO_P3[3][3] = {
    {3.1277689713618742, -2.2571357625916386, 0.1293667912297653},
    {-1.0910090184377974, 2.4133317103069212, -0.3223226918691247},
    {-0.0260108019385704, -0.5080413317041669, 1.5340521336427371}};

namespace oklab
{
    template <>
    Oklab convertToOklab<P3>(const P3 &p3)
    {
        return p3ToOklab(p3);
    }

    template <>
    P3 convertFromOklab<P3>(const Oklab &oklab)
    {
        return oklabToP3(oklab);
    }

    template <>
    LinearP3 clipToGamut<LinearP3>(const LinearP3 &linearP3)
    {
        return LinearP3{
            std::clamp(linearP3[0], 0.0, 1.0),
            std::clamp(linearP3[1], 0.0, 1.0),
            std::clamp(linearP3[2], 0.0, 1.0)};
    };

    inline LinearP3 p3ToLinearP3(const P3 &p3)
    {
        double r = gammaToLinear(p3[0] / 255.0);
        double g = gammaToLinear(p3[1] / 255.0);
        double b = gammaToLinear(p3[2] / 255.0);

        return {r, g, b};
    }

    inline P3 linearP3ToP3(const LinearP3 &linearP3)
    {
        int r = static_cast<int>(std::round(linearToGamma(linearP3[0]) * 255.0));
        int g = static_cast<int>(std::round(linearToGamma(linearP3[1]) * 255.0));
        int b = static_cast<int>(std::round(linearToGamma(linearP3[2]) * 255.0));

        return P3{r, g, b};
    }

    inline Oklab linearP3ToOklab(const LinearP3 &linearP3)
    {
        LMS lms = multiplyMatrix(P3_TO_LMS, linearP3);
        Oklab oklab = lmsToOklab(lms);
        return oklab;
    }

    Oklab p3ToOklab(const P3 &p3)
    {
        double r = gammaToLinear(p3[0] / 255.0);
        double g = gammaToLinear(p3[1] / 255.0);
        double b = gammaToLinear(p3[2] / 255.0);

        return linearP3ToOklab({r, g, b});
    }

    inline LinearP3 oklabToLinearP3(const Oklab &oklab)
    {
        LMS lms = oklabToLms(oklab);
        LinearP3 p3 = multiplyMatrix(LMS_TO_P3, lms);
        return p3;
    }

    template <>
    bool isInGamut<LinearP3>(const LinearP3 &linearP3)
    {
        return linearP3[0] >= 0.0 && linearP3[0] <= 1.0 &&
               linearP3[1] >= 0.0 && linearP3[1] <= 1.0 &&
               linearP3[2] >= 0.0 && linearP3[2] <= 1.0;
    }

    template <>
    LinearP3 oklabToLinearColor<LinearP3>(const Oklab &oklab)
    {
        return oklabToLinearP3(oklab);
    }

    template <>
    P3 linearColorToColor<LinearP3, P3>(const LinearP3 &linearP3)
    {
        return linearP3ToP3(linearP3);
    }

    template <>
    Oklab linearColorToOklab<LinearP3>(const LinearP3 &linearP3)
    {
        return linearP3ToOklab(linearP3);
    }

    P3 oklabToP3(const Oklab &oklab)
    {
#ifdef MAPPING_CSS4
        return performCssGamutMapping<P3, LinearP3>(oklab);
#elif MAPPING_CLAMP
        LinearP3 linearP3 = oklabToLinearP3(oklab);
        LinearP3 clippedLinearP3 = clipToGamut(linearP3);
        return linearP3ToP3(clippedLinearP3);
#else
        return linearP3ToP3(oklabToLinearP3(oklab));
#endif
    }
}