
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "vec3.h"

const point3 sphere_orig = {0, 0, -1};

int main() {
    hittable_list world;

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<dielectric>(1.5);
    auto material_bubble = std::make_shared<dielectric>(1 / 1.5);
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);

    world.add(std::make_unique<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_unique<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(std::make_unique<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(std::make_unique<sphere>(point3(-1.0, 0.0, -1.0), 0.45, material_bubble));
    world.add(std::make_unique<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
}
