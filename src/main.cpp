
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

const point3 sphere_orig = {0, 0, -1};

int main() {

    // World
    hittable_list world;

    world.add(std::make_unique<sphere>(point3(0, 0.5, -2), 1));
    world.add(std::make_unique<sphere>(point3(0, -100.5, -1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1000;

    cam.render(world);
}
