#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, const vec3& pixel_color) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    int rbyte = static_cast<int>(r * 255.999);
    int gbyte = static_cast<int>(g * 255.999);
    int bbyte = static_cast<int>(b * 255.999);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif