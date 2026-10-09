#pragma once

#include <array>
#include <cmath>
#include <iostream>

#include "utilities.h"

class vec3 {
public:
    std::array<double, 3> e;
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    vec3(double a) : e{a, a, a} {}

    [[nodiscard]] constexpr double x() const { return e[0]; }
    [[nodiscard]] constexpr double y() const { return e[1]; }
    [[nodiscard]] constexpr double z() const { return e[2]; }

    [[nodiscard]] vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
    [[nodiscard]] double operator[](std::size_t i) const { return e[i]; }
    [[nodiscard]] double& operator[](std::size_t i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) { return *this *= 1 / t; }

    [[nodiscard]] double length() const { return std::sqrt(length_squared()); }

    [[nodiscard]] double length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    static vec3 random() { return {random_double(), random_double(), random_double()}; }

    static vec3 random(double min, double max) {
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }
};

using point3 = vec3;

// vec3 operations

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline vec3 operator*(double t, const vec3& v) { return {t * v.e[0], t * v.e[1], t * v.e[2]}; }

inline vec3 operator*(const vec3& v, double t) { return t * v; }

inline vec3 operator/(const vec3& v, double t) { return (1 / t) * v; }

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return {
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0],
    };
}

inline vec3 unit_vector(const vec3& v) { return v / v.length(); }

// Uses rejection method
inline vec3 random_unit_vector() {
    while (true) {
        vec3 p = vec3::random(-1, 1);
        double lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0) {
        // Random vector is in same hemisphere as normal
        return on_unit_sphere;
    } else
        return -on_unit_sphere;
}