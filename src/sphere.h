#pragma once

#include "hittable.h"
#include "material.h"
#include "onb.h"
#include "utilities.h"
#include "vec3.h"

#include <cmath>
#include <memory>
#include <utility>

class sphere : public hittable {
public:
    sphere(const point3& center_, double radius_, std::shared_ptr<material> mat_)
        : center(center_), radius(std::fmax(0, radius_)), mat(std::move(mat_)) {}

    /* Derived from quadratic formula. */
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        double a = r.direction().length_squared();
        double h = dot(r.direction(), oc);
        double c = oc.length_squared() - radius * radius;
        double D = h * h - a * c;

        if (D < 0)
            return false;

        double sqrtD = std::sqrt(D);

        /* Find nearest root in acceptable range. */
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
        rec.mat = mat;

        return true;
    }

    [[nodiscard]] double pdf_value(const point3& origin, const vec3& direction) const override {
        ray r = {origin, direction};
        hit_record rec;
        if (!hit(r, interval(0.001, infinity), rec))
            return 0;

        double d_squared = (center - origin).length_squared();

        if (d_squared < radius * radius) {
            /* Default value for when origin is inside the sphere. */
            return 0;
        }

        double cos_max_angle = std::sqrt(1 - radius * radius / d_squared);
        double omega = 2 * pi * (1 - cos_max_angle);

        return 1 / omega;
    }

    [[nodiscard]] vec3 random(const point3& origin) const override {
        /* Samples a direction from a cone. */
        vec3 direction = center - origin;
        onb basis = {direction};

        double d_squared = direction.length_squared();

        if (d_squared <= radius * radius) {
            /* Default value for when origin is inside the sphere. */
            return {1, 0, 0};
        }

        return basis.transform(random_to_sphere(radius, d_squared));
    }

private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
};