#pragma once

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, const vec3& pixel_color) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    static const interval intensity(0, 0.9999);
    int rbyte = static_cast<int>(256 * intensity.clamp(r));
    int gbyte = static_cast<int>(256 * intensity.clamp(g));
    int bbyte = static_cast<int>(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}