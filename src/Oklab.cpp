#include "Oklab.h"

#include <cmath>
#include <algorithm>

const Matrix3x3 linearSrgbToOklabMatrix = {{{0.4122214694707629f, 0.5363325372617349f, 0.0514459932675022f},
                                            {0.2119034958178251f, 0.6806995506452345f, 0.1073969535369406f},
                                            {0.0883024591900564f, 0.2817188391361215f, 0.6299787016738222f}}};

const Matrix3x3 oklabToLinearSrgbMatrix = {{{4.0767416360759592f, -3.3077115392580625f, 0.2309699031821046f},
                                            {-1.2684379732850315f, 2.6097573492876882f, -0.3413193760026572f},
                                            {-0.0041960761386755f, -0.7034186179359361f, 1.7076146940746113f}}};

const Matrix3x3 linearToOklabMatrix = {{{0.210454268309314f, 0.7936177747023054f, -0.0040720430116193f},
                                        {1.9779985324311684f, -2.4285922420485799f, 0.450593709617411f},
                                        {0.0259040424655478f, 0.7827717124575296f, -0.8086757549230774f}}};

const Matrix3x3 oklabToLinearMatrix = {{{1.0f, 0.3963377773761749f, 0.2158037573099136f},
                                        {1.0f, -0.1055613458156586f, -0.0638541728258133f},
                                        {1.0f, -0.0894841775298119f, -1.2914855480194092f}}};

inline float eotf(float value)
{
    return (std::abs(value) <= 0.04045f) ? value / 12.92f : std::copysign(std::pow((std::abs(value) + 0.055f) / 1.055f, 2.4f), value);
}

inline float oetf(float value)
{
    float sign = value < 0 ? -1 : 1;
    float absValue = std::abs(value);
    return (absValue <= 0.0031308f) ? value * 12.92f : sign * (1.055f * std::pow(absValue, 1 / 2.4f) - 0.055f);
}

float constrainAngle(float angle)
{
    return std::fmod((std::fmod(angle, 360.0f) + 360.0f), 360.0f);
}

RGB scaleTo255(const GsRGB &color)
{
    RGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](float v)
                   { return static_cast<int>(std::round(v * 255)); });
    return result;
}

GsRGB scaleTo01(const RGB &color)
{
    GsRGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](int v)
                   { return v / 255.0f; });
    return result;
}

bool inGamut(const GsRGB &color)
{
    return std::all_of(color.begin(), color.end(), [](float v)
                       { return v >= 0.0f && v <= 1.0f; });
}

GsRGB clip(const GsRGB &color)
{
    GsRGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](float v)
                   { return std::min(std::max(v, 0.0f), 1.0f); });
    return result;
}

GsRGB srgbGammaToLinear(const GsRGB &color)
{
    GsRGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](float value)
                   { return eotf(value); });
    return result;
}

GsRGB srgbLinearToGamma(const GsRGB &color)
{
    GsRGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](float value)
                   { return oetf(value); });
    return result;
}

Oklab linearSrgbToOklab(const LsRGB &color)
{
    LsRGB lms = multiply(linearSrgbToOklabMatrix, color);
    lms = elementwiseCbrt(lms); // cube root preserving sign
    return multiply(linearToOklabMatrix, lms);
}

LsRGB oklabToLinearSrgb(const Oklab &color)
{
    LsRGB lms = multiply(oklabToLinearMatrix, color);
    lms = elementwisePow(lms, 3); // cube
    return multiply(oklabToLinearSrgbMatrix, lms);
}

Oklab oklchToOklab(const Oklch &color)
{
    if (std::isnan(color[2]))
    {
        return {color[0], 0, 0};
    }
    else
    {
        float hue = constrainAngle(color[2]);
        float angle = hue * M_PI / 180.0f;
        return {color[0], std::cos(angle) * color[1], std::sin(angle) * color[1]};
    }
}

Oklch oklabToOklch(const Oklab &color)
{
    float epsilon = 0.0002f;
    if (std::abs(color[1]) < epsilon && std::abs(color[2]) < epsilon)
    {
        return {color[0], 0, NAN};
    }
    else
    {
        return {color[0], std::sqrt(color[1] * color[1] + color[2] * color[2]), constrainAngle(std::atan2(color[2], color[1]) * 180.0f / M_PI)};
    }
}

float delta(const Oklab &c1, const Oklab &c2)
{
    float dl = c1[0] - c2[0];
    float da = c1[1] - c2[1];
    float db = c1[2] - c2[2];
    return std::sqrt(dl * dl + da * da + db * db);
}

RGB interpolateColor(const RGB &c1, const RGB &c2, float t)
{
    Oklab c1Lab = linearSrgbToOklab(srgbGammaToLinear(scaleTo01(c1)));
    Oklab c2Lab = linearSrgbToOklab(srgbGammaToLinear(scaleTo01(c2)));

    Oklab cLab = {
        c1Lab[0] * (1 - t) + c2Lab[0] * t,
        c1Lab[1] * (1 - t) + c2Lab[1] * t,
        c1Lab[2] * (1 - t) + c2Lab[2] * t};

    Oklch cLch = oklabToOklch(cLab);

    if (cLch[0] >= 1)
    {
        return {255, 255, 255};
    }

    if (cLch[0] <= 0)
    {
        return {0, 0, 0};
    }

    LsRGB cLinearRGB = oklabToLinearSrgb(cLab);

    if (inGamut(cLinearRGB))
    {
        return scaleTo255(srgbLinearToGamma(cLinearRGB));
    }

    float JUST_NOT_DISCERNABLE = 0.02f;
    float EPSILON = 0.0001f;

    GsRGB clippedLinearRGB = clip(cLinearRGB);
    Oklab clippedOklab = linearSrgbToOklab(clippedLinearRGB);
    float E = delta(clippedOklab, cLab);

    if (E < JUST_NOT_DISCERNABLE)
    {
        return scaleTo255(srgbLinearToGamma(clippedLinearRGB));
    }

    float minChroma = 0;
    float maxChroma = cLch[1];
    bool isMinChromaInGamut = true;
    float chroma = (minChroma + maxChroma) / 2;

    while (maxChroma - minChroma > EPSILON)
    {
        chroma = (minChroma + maxChroma) / 2;

        Oklch currentLch = cLch;
        currentLch[1] = chroma;
        Oklab currentLab = oklchToOklab(currentLch);
        LsRGB currentLinearRGB = oklabToLinearSrgb(currentLab);

        bool isCurrentInRGBGamut = inGamut(currentLinearRGB);

        if (isMinChromaInGamut && isCurrentInRGBGamut)
        {
            minChroma = chroma;
        }
        else
        {
            currentLinearRGB = clip(currentLinearRGB);
            clippedOklab = linearSrgbToOklab(currentLinearRGB);
            E = delta(clippedOklab, currentLab);

            if (E < JUST_NOT_DISCERNABLE)
            {
                if (JUST_NOT_DISCERNABLE - E < EPSILON)
                {
                    break;
                }
                else
                {
                    isMinChromaInGamut = false;
                    minChroma = chroma;
                }
            }
            else
            {
                maxChroma = chroma;
                continue;
            }
        }
    }

    return scaleTo255(srgbLinearToGamma(clippedLinearRGB));
}
