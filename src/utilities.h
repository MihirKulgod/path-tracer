#pragma once

#include <limits>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degs_to_rads(double degs) { return degs * pi / 180; }

inline double random_double() {
    // Returns a random double in [0, 1)
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random double in [min, max)
    return min + (max - min) * random_double();
}