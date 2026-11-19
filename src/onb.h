#pragma once
#include "vec3.h"

class onb {
public:
    onb(const vec3& w_) : w(unit_vector(w_)) {
        vec3 a = (std::abs(w.x()) < 0.9) ? vec3(1, 0, 0) : vec3(0, 1, 0);
        v = unit_vector(cross(w, a));
        u = cross(w, v);
    }

    [[nodiscard]] vec3 transform(const vec3& local) const {
        return local.x() * u + local.y() * v + local.z() * w;
    }

private:
    vec3 u, v, w;
};