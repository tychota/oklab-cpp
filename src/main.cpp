#include "Oklab.h"

#include <vector>
#include <string>
#include <iostream>

int main()
{
    RGB color1 = {255, 0, 0}; // Red
    RGB color2 = {0, 0, 255}; // Blue

    float t = 0.5;
    RGB result = interpolateColor(color1, color2, t);

    std::cout << "Interpolated Color: ("
              << result[0] << ", "
              << result[1] << ", "
              << result[2] << ")" << std::endl;

    return 0;
}
