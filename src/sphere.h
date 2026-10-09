#pragma once

#include "hittable.h"
#include "vec3.h"

#include <cmath>

class sphere : public hittable {
public:
    sphere(const point3& center_, double radius_)
        : center(center_), radius(std::fmax(0, radius_)) {}

    // Derived from quadratic formula
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        double a = r.direction().length_squared();
        double h = dot(r.direction(), oc);
        double c = oc.length_squared() - radius * radius;
        double D = h * h - a * c;

        if (D < 0)
            return false;

        double sqrtD = std::sqrt(D);

        // Find nearest root in acceptable range
        double root = (h - sqrtD) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtD) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};