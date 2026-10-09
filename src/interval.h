#pragma once

#include "utilities.h"

class interval {
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {}

    interval(double min_, double max_) : min(min_), max(max_) {}

    [[nodiscard]] double size() const { return max - min; }

    [[nodiscard]] bool contains(double x) const { return min <= x && x <= max; }

    [[nodiscard]] bool surrounds(double x) const { return min < x && x < max; }

    [[nodiscard]] double clamp(double x) const {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);