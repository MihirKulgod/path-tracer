
#include <iostream>
#include <memory>

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

    // Small light above the spheres; dark sky so diffuse surfaces are lit mostly by it.
    auto light_material = std::make_shared<diffuse_light>(color(15));
    auto light_sphere = std::make_shared<sphere>(point3(0, 1.4, -0.5), 0.3, light_material);
    world.add(light_sphere);
    lights.add(light_sphere);

    // Verify hittable_list::random / pdf_value with two lights of different size and distance:
    // every sample should hit a light, picks should split ~50/50, and pdf should integrate to ~1.
    {
        const auto second_light = std::make_shared<sphere>(point3(2, 1, -1), 0.5, light_material);
        hittable_list test_lights;
        test_lights.add(light_sphere);
        test_lights.add(second_light);
        const point3 origin(0, 0, 1);

        int misses = 0;
        int first_hits = 0;
        for (int i = 0; i < 100000; i++) {
            hit_record rec;
            const ray r(origin, test_lights.random(origin));
            if (!test_lights.hit(r, interval(0.001, infinity), rec)) {
                misses++;
            } else if (light_sphere->hit(r, interval(0.001, infinity), rec)) {
                first_hits++;
            }
        }
        std::clog << "misses: " << misses << " / 100000\n";
        std::clog << "first light share: " << first_hits / 1000.0 << "% (expect ~50%)\n";

        double sum = 0;
        for (int i = 0; i < 1000000; i++) {
            sum += test_lights.pdf_value(origin, random_unit_vector()) * 4 * pi;
        }
        std::clog << "pdf integral: " << sum / 1000000 << " (expect ~1)\n";
    }
    return 0;

    auto ground_material = std::make_shared<lambertian>(color(0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100, ground_material));

    auto diffuse_material = std::make_shared<lambertian>(color(0.6, 0.3, 0.3));
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5, diffuse_material));

    auto metal_material = std::make_shared<metal>(color(0.8), 0.0);
    world.add(std::make_shared<sphere>(point3(1.1, 0, -1), 0.5, metal_material));

    auto glass_material = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(-1.1, 0, -1), 0.5, glass_material));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 20;

    cam.vfov = 40;
    cam.lookfrom = point3(0, 1, 3);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    cam.sky_start = color(0);
    cam.sky_end = color(0.12, 0.09, 0.1);

    cam.render(world, lights);
}
