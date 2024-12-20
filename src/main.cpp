#include <iostream>

#include "color.h"
#include "vec3.h"

int main()
{
    const int IMAGE_WIDTH   = 256;
    const int IMAGE_HEIGHT  = 256;

    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    for(float y = 0.; y < IMAGE_HEIGHT; y++)
    {
        for(float x = 0.; x < IMAGE_WIDTH; x++)
        {
            color pixel_color = color(
                x / IMAGE_WIDTH,
                y / IMAGE_HEIGHT,
                0.
            );

            write_color(std::cout, pixel_color);
        }

        const int prog_len = 30;
        const float prog_ratio = y / IMAGE_HEIGHT;
        std::clog << "\r[" << std::string(prog_ratio*prog_len, '=') << ">" << std::string(prog_len - prog_ratio*prog_len, ' ') << "]" << prog_ratio*100 << "%";
    }

    return 0;
}