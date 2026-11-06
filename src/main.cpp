
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "utilities.h"
#include "vec3.h"

int main() {
    hittable_list world;
    hittable_list lights;

    // Central light, surrounded by one sphere of each other material
    auto light_material = std::make_shared<diffuse_light>(color(6, 0.95, 0.95));
    auto light_sphere = std::make_shared<sphere>(point3(0, 0, 0), 1.0, light_material);
    world.add(light_sphere);
    lights.add(light_sphere);

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