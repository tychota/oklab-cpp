#include <iostream>
#include "args.hxx"
#include "ColorConversions.h"
#include "ColorTypes.h"

using namespace oklab;

// Example function to parse RGB input
RGB parseRGB(const std::string &input)
{
    std::istringstream stream(input);
    int r, g, b;
    char sep;
    stream >> r >> g >> b;
    return RGB{r, g, b};
}

// Example function to parse P3 input (similar to RGB for simplicity)
P3 parseP3(const std::string &input)
{
    std::istringstream stream(input);
    int r, g, b;
    char sep;
    stream >> r >> g >> b;
    return P3{r, g, b};
}

int main(int argc, char **argv)
{
    args::ArgumentParser parser("Color transformation CLI application.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Group commands(parser, "commands", args::Group::Validators::AtMostOne);

    args::Command rgbToP3Cmd(commands, "rgbToP3", "Convert RGB to P3 color space", [&](args::Subparser &sp)
                             {
        args::ValueFlag<std::string> inputColor(sp, "inputColor", "The input RGB color (format: 'r g b')", {"inputColor"}, args::Options::Required);
        sp.Parse();
        RGB rgb = parseRGB(args::get(inputColor));
        Oklab oklab = rgbToOklab(rgb);
        P3 p3 = oklabToP3(oklab);
        std::cout << "Converted P3: " << p3[0] << ", " << p3[1] << ", " << p3[2] << std::endl; });

    args::Command p3ToRgbCmd(commands, "P3ToRgb", "Convert P3 to RGB color space", [&](args::Subparser &sp)
                             {
        args::ValueFlag<std::string> inputColor(sp, "inputColor", "The input P3 color (format: 'r g b')", {"inputColor"}, args::Options::Required);
        sp.Parse();
        P3 p3 = parseP3(args::get(inputColor));
        std::cout << "Parsed P3: " << p3[0] << ", " << p3[1] << ", " << p3[2] << std::endl;
        Oklab oklab = p3ToOklab(p3);
        RGB rgb = oklabToRgb(oklab);
        std::cout << "Converted RGB: " << rgb[0] << ", " << rgb[1] << ", " << rgb[2] << std::endl; });

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help &)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    return 0;
}