#include "include/common.h"

#include "include/color.h"
#include "include/hittable_list.h"
#include "include/sphere.h"
#include "include/camera.h"
#include "include/material.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth)
{
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);  // Interp traverse on the y axis
    return interp3(white, sky_blue, t);
}

int main()
{

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world;

    auto material_ground   = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_diff     = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_metal    = make_shared<metal>(color(0.8, 0.8, 0.8), 0.2);
    auto material_bronze   = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_diff));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.5), 0.5, material_bronze));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.5), 0.5, material_metal));

    // Camera
    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = (image_height - 1); j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}