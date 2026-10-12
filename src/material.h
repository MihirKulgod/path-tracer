#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter([[maybe_unused]] const ray& r_in, [[maybe_unused]] const hit_record& rec,
                         [[maybe_unused]] color& attenuation,
                         [[maybe_unused]] ray& scattered) const {
        return false;
    }
};