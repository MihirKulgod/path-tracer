#include "color.h"

#include <iostream>

int main() {
    int image_width = 256;
    int image_height = 256;

    // Header of PPM image format
    // P3 means colors are in ASCII, 255 is max colour, dimensions are provided too
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            color pixel_color = {
                static_cast<double>(i) / (image_width - 1),
                static_cast<double>(j) / (image_height - 1),
                0,
            };

            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\r\033[KDone.\n";
    return 0;
}
