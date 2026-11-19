
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "utilities.h"
#include "vec3.h"

int main() {

    hittable_list world;
    hittable_list lights;

    auto light_material = std::make_shared<diffuse_light>(color(6, 0.95, 0.95));
    auto light_sphere = std::make_shared<sphere>(point3(0, 0, 0), 0.3, light_material);
    world.add(light_sphere);
    lights.add(light_sphere);

    // Verify sphere::random: every sampled direction must hit the sphere.
    {
        const point3 origin(0, 5, 0);
        int misses = 0;
        for (int i = 0; i < 100000; i++) {
            hit_record rec;
            const ray r(origin, light_sphere->random(origin));
            if (!light_sphere->hit(r, interval(0.001, infinity), rec)) {
                misses++;
            }
        }
        std::clog << "misses: " << misses << " / 100000\n";
    }

    // Verify sphere::pdf_value: averaging pdf * 4pi over uniform sphere directions should give ~1.
    {
        const point3 origin(0, 5, 0);
        double sum = 0;
        for (int i = 0; i < 1000000; i++) {
            sum += light_sphere->pdf_value(origin, random_unit_vector()) * 4 * pi;
        }
        std::clog << "pdf integral: " << sum / 1000000 << " (expect ~1)\n";
    }

    // Verify random_to_sphere: local z should be uniform over [cos_max, 1], so bins roughly equal.
    {
        const double radius = 0.3;
        const double d_squared = 25;
        const double cos_max = std::sqrt(1 - radius * radius / d_squared);
        std::array<int, 10> bins{};
        for (int i = 0; i < 100000; i++) {
            const double z = random_to_sphere(radius, d_squared).z();
            bins.at(static_cast<std::size_t>((1 - z) / (1 - cos_max) * 10))++;
        }
        std::clog << "z bins (expect ~10000 each):";
        for (const int count : bins) {
            std::clog << ' ' << count;
        }
        std::clog << '\n';
    }
    return 0;

    auto metal_material = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
    world.add(std::make_shared<sphere>(point3(2.2, 0, 1.3), 1.0, metal_material));

    auto diffuse_material = std::make_shared<lambertian>(color(0.6, 0.3, 0.3));
    world.add(std::make_shared<sphere>(point3(0, 0, -2.5), 1.0, metal_material));

    auto glass_material = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(-2.2, 0, 1.3), 1.0, metal_material));
    world.add(std::make_shared<sphere>(point3(0, 2.5, 0), 0.7, glass_material));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 960;
    cam.samples_per_pixel = 500;
    cam.max_depth = 40;

    cam.vfov = 40;
    cam.lookfrom = point3(0, 5, -0.0001);
    cam.lookat = point3(0, 0, -0.5);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    cam.sky_start = color(0);
    cam.sky_end = color(0.12, 0.09, 0.1);

    cam.render(world, lights);
}