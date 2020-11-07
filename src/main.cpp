#include "include/common.h"

#include "include/color.h"
#include "include/hittable_list.h"
#include "include/sphere.h"
#include "include/moving_sphere.h"
#include "include/camera.h"
#include "include/material.h"
#include "include/bvh.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

#include <iostream>

hittable_list demo_scene(int gridsize = 11) {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -gridsize; a < gridsize; a++) {
        for (int b = -gridsize; b < gridsize; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            auto center2 = center + vec3(0, random_double(0, 0.5), 0);

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<moving_sphere>(center, center, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    //metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<moving_sphere>(center, center, 0.0, 1.0, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<moving_sphere>(center, center, 0.0, 1.0, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

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
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;  // Maximum reflection depth

    uint8_t* image_data = new uint8_t[image_width * image_height * 3];
    int image_index = 0;

    // World
    //bvh_node world(demo_scene(9), 0, 1.0);
    hittable_list world;

    auto material_ground   = make_shared<lambertian>(color(0.1, 0.8, 0.2));
    auto material_diff     = make_shared<lambertian>(color(0.9, 0.1, 0.7));
    auto material_metal    = make_shared<metal>(color(0.8, 0.8, 0.8), 0.2);
    auto material_bronze   = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);
    auto material_glass    = make_shared<dielectric>(1.5);

    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_diff));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_bronze));

    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_glass));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_glass));

    world.add(make_shared<Sphere>(point3(0.0, 1.0, -1.5), 0.5, material_metal));

    bvh_node bvh_world(world, 0, 1.0);

    // Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    double dist_to_focus = 10;
    camera cam(lookfrom, lookat, vup, 25, aspect_ratio, 0.1, dist_to_focus, 0.0, 1.0);

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
            write_data_color(image_data, &image_index, pixel_color, samples_per_pixel);
            //write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone scanning.\n";

    std::cout << "Saving output...\n";

    stbi_write_png("image.png", image_width, image_height, 3, image_data, sizeof(uint8_t) * image_width * 3);

    std::cerr << "Done saving output.\n";
}