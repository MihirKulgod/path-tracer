#pragma once

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "utilities.h"
#include "vec3.h"

class camera {
public:
    double aspect_ratio = 1;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    // Vertical field of view (degrees)
    double vfov = 90;
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

    double defocus_angle = 0;
    double focus_dist = 10;

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
    vec3 u, v, w;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        double theta = degs_to_rads(vfov);
        double h = std::tan(theta / 2);
        double viewport_height = 2 * h * focus_dist;
        double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        vec3 viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        double defocus_radius = focus_dist * std::tan(degs_to_rads(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    [[nodiscard]] ray get_ray(int i, int j) const {
        vec3 offset = sample_square();
        point3 pixel_sample =
            pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        vec3 ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    [[nodiscard]] vec3 sample_square() const {
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    [[nodiscard]] point3 defocus_disk_sample() const {
        point3 p = random_in_unit_disc();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    [[nodiscard]] color ray_color(const ray& r, int depth, const hittable& world) const {
        // On hitting bounce limit, return black
        if (depth <= 0)
            return {0};
        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return {0};
        }

        vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1 - a) * color(1) + a * color(0.3, 0.7, 1.0);
    }
};