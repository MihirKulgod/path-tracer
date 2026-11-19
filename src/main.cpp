
#include <memory>

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "vec3.h"

int main() {
    hittable_list world;
    hittable_list lights;

    // Small light above the spheres; dark sky so diffuse surfaces are lit mostly by it.
    auto light_material = std::make_shared<diffuse_light>(color(15));
    auto light_sphere = std::make_shared<sphere>(point3(0, 1.4, -0.5), 0.3, light_material);
    world.add(light_sphere);
    lights.add(light_sphere);

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
