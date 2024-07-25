#pragma once

#include "ColorTypes.h"

#include "../ColorUtils.h"
#include "../OkLxx.h"

#ifdef DEBUG_LOOP_COUNT
#include <iostream>
#include <iomanip>
#endif

/**
 * @file gamutMapping/CSS4.h
 * @brief Provides functions for mapping colors using the CSS4 gamut.
 *
 * This file provides functions for mapping colors to the CSS4 gamut algorithm.
 *
 */

namespace oklab
{
    template <typename LinearColorType>
    bool isInGamut(const LinearColorType &color);

    template <typename LinearColorType>
    LinearColorType oklabToLinearColor(const Oklab &oklab);

    template <typename LinearColorType, typename ColorType>
    ColorType linearColorToColor(const LinearColorType &linearColor);

    template <typename LinearColorType>
    Oklab linearColorToOklab(const LinearColorType &linearColor);

    template <typename ColorType, typename LinearColorType>
    ColorType performCssGamutMapping(const Oklab &oklab);

    template <typename ColorType, typename LinearColorType>
    ColorType performCssGamutMapping(const Oklab &oklab)
    {
        Oklch oklch = oklabToOklch(oklab);

        if (oklch[0] >= 1)
        {
            return ColorType{255, 255, 255};
        }

        if (oklch[0] <= 0)
        {
            return ColorType{0, 0, 0};
        }

        LinearColorType linearColor = oklabToLinearColor<LinearColorType>(oklab);

        if (isInGamut(linearColor))
        {
            return linearColorToColor<LinearColorType, ColorType>(linearColor);
        }

        const double JUST_NON_DISCERNIBLE = 0.02;
        const double EPSILON = 0.0001;

        LinearColorType clippedLinearColor = clipToGamut<LinearColorType>(linearColor);
        Oklab clippedOklab = linearColorToOklab<LinearColorType>(clippedLinearColor);

        double E = deltaE(clippedLinearColor, oklab);

        if (E < JUST_NON_DISCERNIBLE)
        {
            return linearColorToColor<LinearColorType, ColorType>(clippedLinearColor);
        }

        double minChroma = 0.0;
        double maxChroma = oklch[1];

        bool isMinChromaInGamut = true;

        Oklch optimisedOklch = {oklch[0], oklch[1], oklch[2]};

#ifdef DEBUG_LOOP_COUNT
        int loopCount = 0;
#endif

        while (maxChroma - minChroma > EPSILON)
        {
#ifdef DEBUG_LOOP_COUNT
            ++loopCount;
#endif
            double optimisedChroma = (minChroma + maxChroma) / 2.0;

            optimisedOklch = Oklch{oklch[0], optimisedChroma, oklch[2]};
            Oklab optimisedOklab = oklchToOklab(optimisedOklch);
            LinearColorType optimisedLinearColor = oklabToLinearColor<LinearColorType>(optimisedOklab);

            if (isMinChromaInGamut && isInGamut<LinearColorType>(optimisedLinearColor))
            {
                minChroma = optimisedChroma;
#ifdef DEBUG_LOOP_COUNT
                std::cerr << "Loop " << loopCount << ": Chroma adjusted to "
                          << std::fixed << std::setprecision(4) << optimisedChroma
                          << " - Min chroma is in Gamut. Oklab ("
                          << std::fixed << std::setprecision(4) << optimisedOklab[0] << ", "
                          << std::fixed << std::setprecision(4) << optimisedOklab[1] << ", "
                          << std::fixed << std::setprecision(4) << optimisedOklab[2] << ")\n";
#endif
            }
            else
            {
                clippedLinearColor = clipToGamut<LinearColorType>(optimisedLinearColor);
                Oklab clippedOklab = linearColorToOklab<LinearColorType>(clippedLinearColor);

                E = deltaE(clippedOklab, optimisedOklab);

                if (E < JUST_NON_DISCERNIBLE)
                {
                    if (JUST_NON_DISCERNIBLE - E < EPSILON) [[unlikely]]
                    {
#ifdef DEBUG_LOOP_COUNT
                        std::cerr << "Loop " << loopCount << ": Chroma adjusted to "
                                  << std::fixed << std::setprecision(4) << optimisedChroma
                                  << " - Optimised TADA. Oklab ("
                                  << std::fixed << std::setprecision(4) << optimisedOklab[0] << ", "
                                  << std::fixed << std::setprecision(4) << optimisedOklab[1] << ", "
                                  << std::fixed << std::setprecision(4) << optimisedOklab[2] << ")\n";
#endif
                        break;
                    }
                    else
                    {
                        isMinChromaInGamut = false;
                        minChroma = optimisedChroma;
#ifdef DEBUG_LOOP_COUNT
                        std::cerr << "Loop " << loopCount << ": Chroma adjusted to "
                                  << std::fixed << std::setprecision(4) << optimisedChroma
                                  << " - Min chroma is out of Gamut - Update Min Chroma. Oklab ("
                                  << std::fixed << std::setprecision(4) << optimisedOklab[0] << ", "
                                  << std::fixed << std::setprecision(4) << optimisedOklab[1] << ", "
                                  << std::fixed << std::setprecision(4) << optimisedOklab[2] << ")\n";
#endif
                    }
                }
                else [[unlikely]]
                {
                    maxChroma = optimisedChroma;
#ifdef DEBUG_LOOP_COUNT
                    std::cerr << "Loop " << loopCount << ": Chroma adjusted to "
                              << std::fixed << std::setprecision(4) << optimisedChroma
                              << " - Min chroma is out of Gamut - Update Max Chroma. Oklab ("
                              << std::fixed << std::setprecision(4) << optimisedOklab[0] << ", "
                              << std::fixed << std::setprecision(4) << optimisedOklab[1] << ", "
                              << std::fixed << std::setprecision(4) << optimisedOklab[2] << ")\n";
#endif
                    continue;
                }
            }
        }

#ifdef DEBUG_LOOP_COUNT
        std::cerr << "CSS4 gamut mapping loop executed " << loopCount << " times" << "\n ";
#endif

        return linearColorToColor<LinearColorType, ColorType>(clipToGamut<LinearColorType>(clippedLinearColor));
    }
}
