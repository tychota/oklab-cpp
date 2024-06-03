#include "Oklab.h"
#include <cmath>
#include <algorithm>

const Matrix3x3 linearSrgbToOklabMatrix = {{{0.4122214694707629, 0.5363325372617349, 0.0514459932675022},
                                            {0.2119034958178251, 0.6806995506452345, 0.1073969535369406},
                                            {0.0883024591900564, 0.2817188391361215, 0.6299787016738222}}};

const Matrix3x3 oklabToLinearSrgbMatrix = {{{4.0767416360759592, -3.3077115392580625, 0.2309699031821046},
                                            {-1.2684379732850315, 2.6097573492876882, -0.3413193760026572},
                                            {-0.0041960761386755, -0.7034186179359361, 1.7076146940746113}}};

const Matrix3x3 linearToOklabMatrix = {{{0.210454268309314, 0.7936177747023054, -0.0040720430116193},
                                        {1.9779985324311684, -2.4285922420485799, 0.450593709617411},
                                        {0.0259040424655478, 0.7827717124575296, -0.8086757549230774}}};

const Matrix3x3 oklabToLinearMatrix = {{{1.0, 0.3963377773761749, 0.2158037573099136},
                                        {1.0, -0.1055613458156586, -0.0638541728258133},
                                        {1.0, -0.0894841775298119, -1.2914855480194092}}};

inline double eotf(double value)
{
    return (std::abs(value) <= 0.04045) ? value / 12.92 : std::copysign(std::pow((std::abs(value) + 0.055) / 1.055, 2.4), value);
}

inline double oetf(double value)
{
    double sign = value < 0 ? -1 : 1;
    double absValue = std::abs(value);
    return (absValue <= 0.0031308) ? value * 12.92 : sign * (1.055 * std::pow(absValue, 1 / 2.4) - 0.055);
}

double constrainAngle(double angle)
{
    return std::fmod((std::fmod(angle, 360.0) + 360.0), 360.0);
}

RGB scaleTo255(const GsRGB &color)
{
    RGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](double v)
                   { return static_cast<int>(std::round(v * 255)); });
    return result;
}

GsRGB scaleTo01(const RGB &color)
{
    GsRGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](int v)
                   { return v / 255.0; });
    return result;
}

bool inGamut(const GsRGB &color)
{
    return std::all_of(color.begin(), color.end(), [](double v)
                       { return v >= 0.0 && v <= 1.0; });
}

GsRGB clip(const GsRGB &color)
{
    GsRGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](double v)
                   { return std::min(std::max(v, 0.0), 1.0); });
    return result;
}

GsRGB srgbGammaToLinear(const GsRGB &color)
{
    GsRGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](double value)
                   { return eotf(value); });
    return result;
}

GsRGB srgbLinearToGamma(const GsRGB &color)
{
    GsRGB result;
    std::transform(color.begin(), color.end(), result.begin(), [](double value)
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
        double hue = constrainAngle(color[2]);
        double angle = hue * M_PI / 180.0;
        return {color[0], std::cos(angle) * color[1], std::sin(angle) * color[1]};
    }
}

Oklch oklabToOklch(const Oklab &color)
{
    double epsilon = 0.0002;
    if (std::abs(color[1]) < epsilon && std::abs(color[2]) < epsilon)
    {
        return {color[0], 0, NAN};
    }
    else
    {
        return {color[0], std::sqrt(color[1] * color[1] + color[2] * color[2]), constrainAngle(std::atan2(color[2], color[1]) * 180.0 / M_PI)};
    }
}

double delta(const Oklab &c1, const Oklab &c2)
{
    double dl = c1[0] - c2[0];
    double da = c1[1] - c2[1];
    double db = c1[2] - c2[2];
    return std::sqrt(dl * dl + da * da + db * db);
}

RGB interpolateColor(const RGB &c1, const RGB &c2, double t)
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

    double JUST_NOT_DISCERNABLE = 0.02;
    double EPSILON = 0.0001;

    GsRGB clippedLinearRGB = clip(cLinearRGB);
    Oklab clippedOklab = linearSrgbToOklab(clippedLinearRGB);
    double E = delta(clippedOklab, cLab);

    if (E < JUST_NOT_DISCERNABLE)
    {
        return scaleTo255(srgbLinearToGamma(clippedLinearRGB));
    }

    double minChroma = 0;
    double maxChroma = cLch[1];
    bool isMinChromaInGamut = true;
    double chroma = (minChroma + maxChroma) / 2;

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