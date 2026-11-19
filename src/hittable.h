#pragma once

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#include <memory>

class material;

class hit_record {
public:
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t{};
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        /* Check whether ray is in same direction as normal. */
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    /* Returns true if the object was hit, false otherwise. Updates information about the hit in
     * rec. */
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

    /* Given a direction emanating from a given point, returns the Probability Density Function's
     * value. */
    [[nodiscard]] virtual double pdf_value([[maybe_unused]] const point3& origin,
                                           [[maybe_unused]] const vec3& direction) const {
        return 0;
    };

    /* Given a point, generate a random vector in the direction of the object. */
    [[nodiscard]] virtual vec3 random([[maybe_unused]] const point3& origin) const {
        return {1, 0, 0};
    }
};