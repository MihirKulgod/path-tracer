#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter([[maybe_unused]] const ray& r_in, [[maybe_unused]] const hit_record& rec,
                         [[maybe_unused]] color& attenuation,
                         [[maybe_unused]] ray& scattered) const {
        return false;
    }
};

class lambertian : public material {
public:
    lambertian(const color& albedo_) : albedo(albedo_) {}

    bool scatter([[maybe_unused]] const ray& r_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        // This implementation always scatters rays
        vec3 scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& albedo_) : albedo(albedo_) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};