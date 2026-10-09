#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "utilities.h"
#include "vec3.h"

class camera {
public:
    double aspect_ratio = 1;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    void render(const hittable& world) {
        initialize();
        // Header of PPM image format
        // P3 means colors are in ASCII, 255 is max colour, dimensions are provided too
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color * pixel_samples_scale);
            }
        }

        std::clog << "\r\033[KDone.\n";
    }

private:
    int image_height;
    double pixel_samples_scale;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = point3(0, 0, 0);

        // Camera
        double focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
        point3 camera_center = {0, 0, 0};

        vec3 viewport_u = {viewport_width, 0, 0};
        vec3 viewport_v = {0, -viewport_height, 0};

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        vec3 viewport_upper_left =
            camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    [[nodiscard]] ray get_ray(int i, int j) const {
        vec3 offset = sample_square();
        point3 pixel_sample =
            pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        point3 ray_origin = center;
        vec3 ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    [[nodiscard]] vec3 sample_square() const {
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    [[nodiscard]] color ray_color(const ray& r, int depth, const hittable& world) const {
        // On hitting bounce limit, return black
        if (depth <= 0)
            return {0};
        hit_record rec;
        if (world.hit(r, interval(0, infinity), rec)) {
            vec3 direction = random_on_hemisphere(rec.normal);
            return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
        }

        vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1 - a) * color(1) + a * color(0.3, 0.7, 1.0);
    }
};