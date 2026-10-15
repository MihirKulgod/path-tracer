
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "utilities.h"
#include "vec3.h"

const point3 sphere_orig = {0, 0, -1};

int main() {
    hittable_list world;

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<dielectric>(1.50);
    auto material_bubble = std::make_shared<dielectric>(1.00 / 1.50);
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_unique<sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(make_unique<sphere>(point3(0, 0, -1.2), 0.5, material_center));
    world.add(make_unique<sphere>(point3(-1, 0, -1), 0.5, material_left));
    world.add(make_unique<sphere>(point3(-1, 0, -1), 0.4, material_bubble));
    world.add(make_unique<sphere>(point3(1, 0, -1), 0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.render(world);
}
