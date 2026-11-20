
#include <memory>
#include <vector>

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "utilities.h"
#include "vec3.h"

// Small regression scene: one light, a diffuse floor and three material spheres.
void quick_scene(hittable_list& world, hittable_list& lights, camera& cam) {
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

    cam.vfov = 40;
    cam.lookfrom = point3(0, 1, 3);
    cam.lookat = point3(0, 0, -1);
    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;
    cam.sky_start = color(0);
    cam.sky_end = color(0.12, 0.09, 0.1);
}

// Golden spheres floating in a dark void, from right in front of the lens out to the far distance.
// Lights are mostly mid-to-far so they show up through reflections and refractions.
void golden_scene(hittable_list& world, hittable_list& lights, camera& cam) {
    // Palette: golds and ambers, plus neutral glass. Lights are a warm white.
    const color gold(0.95, 0.75, 0.35);
    const color amber(0.9, 0.55, 0.2);
    const color cream(0.9, 0.82, 0.65);
    const color bronze(0.6, 0.38, 0.2);
    const color lamp(1.0, 0.82, 0.55);

    // Track placed spheres so random ones don't overlap anything.
    std::vector<std::pair<point3, double>> placed;
    auto add = [&](const point3& center, double radius, std::shared_ptr<material> mat,
                   bool sample_light) {
        auto s = std::make_shared<sphere>(center, radius, std::move(mat));
        world.add(s);
        if (sample_light)
            lights.add(s);
        placed.emplace_back(center, radius);
    };
    auto light = [&](double strength) { return std::make_shared<diffuse_light>(strength * lamp); };

    // Hero cluster around the focus distance (~9 units away).
    add(point3(-1.8, 0, -9.5), 1.6, std::make_shared<metal>(gold, 0.02), false);
    add(point3(1.6, -0.2, -8.5), 1.3, std::make_shared<dielectric>(1.5), false);
    add(point3(0.2, -1.5, -7.6), 0.8, std::make_shared<lambertian>(cream), false);
    add(point3(4.0, 1.1, -11.5), 1.0, std::make_shared<metal>(bronze, 0.3), false);
    add(point3(-4.6, -1.2, -10.5), 0.9, std::make_shared<lambertian>(amber), false);

    // Small lights tucked between the hero spheres, lighting them and reflecting in them.
    add(point3(0.1, 1.3, -8.4), 0.3, light(12), true);
    add(point3(-0.4, -0.6, -11.5), 0.35, light(10), true);
    add(point3(2.9, -1.6, -10.0), 0.25, light(12), true);
    add(point3(-3.3, 1.9, -8.8), 0.25, light(10), true);

    // Very close foreground spheres, deliberately out of focus. No lights up close.
    add(point3(-1.0, -0.55, -2.2), 0.5, std::make_shared<dielectric>(1.5), false);
    add(point3(1.35, 0.75, -2.9), 0.6, std::make_shared<metal>(gold, 0.0), false);
    add(point3(0.55, -0.5, -1.5), 0.25, std::make_shared<metal>(bronze, 0.1), false);

    // Random scatter from just behind the hero cluster to far away. Spread grows with distance
    // so spheres stay roughly inside the view; size grows too so far ones remain visible.
    for (int attempts = 0; attempts < 2000 && placed.size() < 110; attempts++) {
        const double distance = random_double(13, 160);
        const point3 center(random_double(-0.6, 0.6) * distance,
                            random_double(-0.35, 0.35) * distance, -distance);
        const double radius = random_double(0.4, 1.2) * (1 + distance / 30);

        bool overlaps = false;
        for (const auto& [other_center, other_radius] : placed) {
            if ((center - other_center).length() < radius + other_radius + 0.2)
                overlaps = true;
        }
        if (overlaps)
            continue;

        const double choice = random_double();
        if (choice < 0.25) {
            // Lights stay small and, far away, dim enough to read as golden bokeh rather than
            // clipped white. Only nearer lights are worth aiming at; far ones barely light
            // anything, so they stay world-only and are still found by the material's samples.
            const bool near = distance < 30;
            add(center, radius * 0.35, light(near ? random_double(4, 8) : random_double(0.8, 2.5)),
                near);
        } else if (choice < 0.55) {
            const color tint = random_double() < 0.5 ? gold : amber;
            add(center, radius, std::make_shared<metal>(tint, random_double(0, 0.25)), false);
        } else if (choice < 0.8) {
            add(center, radius, std::make_shared<dielectric>(1.5), false);
        } else {
            const color tint = random_double() < 0.5 ? cream : bronze;
            add(center, radius, std::make_shared<lambertian>(tint), false);
        }
    }

    cam.vfov = 35;
    cam.lookfrom = point3(0, 0.2, 0);
    cam.lookat = point3(0, 0, -9);
    cam.defocus_angle = 1.5;
    cam.focus_dist = 9.0;
    cam.sky_start = color(0);
    cam.sky_end = color(0.012, 0.009, 0.006);
}

int main() {
    hittable_list world;
    hittable_list lights;
    camera cam;

    golden_scene(world, lights, cam);

    // Fast preview settings for composing; raise for the final render.
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 480;
    cam.samples_per_pixel = 16;
    cam.max_depth = 10;
    cam.vup = vec3(0, 1, 0);

    cam.render(world, lights);
}
