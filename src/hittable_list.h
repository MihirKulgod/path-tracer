#pragma once

#include "hittable.h"
#include "interval.h"
#include "utilities.h"

#include <cstddef>
#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;
    hittable_list(std::shared_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object) { objects.push_back(std::move(object)); }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        double closest_so_far = ray_t.max;

        for (const std::shared_ptr<hittable>& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    [[nodiscard]] double pdf_value(const point3& origin, const vec3& direction) const override {
        if (objects.size() == 0)
            return hittable::pdf_value(origin, direction);

        double total = 0;
        for (const std::shared_ptr<hittable>& object : objects) {
            total += object->pdf_value(origin, direction);
        }
        return total / static_cast<double>(objects.size());
    }

    [[nodiscard]] vec3 random(const point3& origin) const override {
        if (objects.size() == 0)
            return hittable::random(origin);

        auto index =
            static_cast<std::size_t>(random_double() * static_cast<double>(objects.size()));
        return objects[index]->random(origin);
    }
};