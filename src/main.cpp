#include "include/common.h"

#include "include/color.h"
#include "include/hittable_list.h"
#include "include/sphere.h"
#include "include/moving_sphere.h"
#include "include/camera.h"
#include "include/material.h"
#include "include/bvh.h"
#include "include/aarect.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

#include <iostream>

// ----------------------------
// |         SCENES           |
// ----------------------------

hittable_list demo_scene(int gridsize = 11) {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

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
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.0, 0.0, 0.0), color(1.0, 1.0, 1.0));

    objects.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list demo_mats() {
    hittable_list world;

    auto material_ground   = make_shared<lambertian>(color(0.1, 0.8, 0.2));
    auto material_diff     = make_shared<lambertian>(color(0.5, 0.0, 0.5));
    auto material_metal    = make_shared<metal>(color(0.8, 0.8, 0.8), 0.2);
    auto material_bronze   = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);
    auto material_glass    = make_shared<dielectric>(1.5);
    auto texture_checker   = make_shared<checker_texture>(color(0.0, 0.0, 0.0), color(1, 1, 1));
    auto material_checker  = make_shared<lambertian>(texture_checker);

    world.add(make_shared<sphere>(point3(0, -1000.5, -1), 1000.0, material_checker));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_diff));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_bronze));

    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_glass));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_glass));

    world.add(make_shared<sphere>(point3(0.0, 1.0, -1.5), 0.5, material_metal));

    return world;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4, 3);
    auto marbtext = make_shared<marble_texture>(10, 3, 10);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(marbtext)));

    return objects;
}

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("texture/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list light_demo() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4, 3);
    auto marbtext = make_shared<marble_texture>(10, 3, 10);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(marbtext)));

    auto difflight = make_shared<diffuse_light>(4 * color(1, 1, 1));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    auto difflight_bright = make_shared<diffuse_light>(10 * color(1, 1, 1));
    objects.add(make_shared<sphere>(point3(0, 7, 0), 1.7, difflight_bright));

    auto material_glass    = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(3, 2, 2), 2.0, material_glass));
    objects.add(make_shared<sphere>(point3(3, 2, 2),-1.9, material_glass));

    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    return objects;
}

// --------------------------
//      /END OF SCENES
// --------------------------

color ray_color(const ray& r, const color& background, const hittable& world, int depth)
{
    hit_record rec;

    if (depth <= 0)
        return color(0, 0, 0);

    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);

    /* Old Sky Color Code */
    //vec3 unit_direction = unit_vector(r.direction());
    //auto t = 0.5 * (unit_direction.y() + 1.0);  // Interp traverse on the y axis
    //return interp3(white, sky_blue, t);
}

int main()
{
    // Image
    auto aspect_ratio = 3.0 / 2.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_depth = 50;  // Maximum reflection depth

    color background(0, 0, 0);

    // World
    hittable_list world;

    point3 lookfrom, lookat;
    auto vfov = 40.0;
    auto aperature = 0.0;

    switch (6) {
    case 0:
        world = demo_mats();
        background = color(0.7, 0.8, 1.0);
        lookfrom = point3(1, 0, 2);
        lookat = point3(0, 0,-1);
        break;
    case 1:
        world = demo_scene();
        background = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        aperature = 0.1;
        break;
    case 2:
        world = two_spheres();
        background = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 3:
        world = two_perlin_spheres();
        background = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 25.0;
        break;
    case 4:
        world = earth();
        background = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 25.0;
        break;
    case 5:
        world = light_demo();
        samples_per_pixel = 400;
        background = color(0, 0, 0);
        lookfrom = point3(26, 3, 6);
        lookat = point3(0, 2, 0);
        vfov = 25.0;
        break;
    case 6:
        world = cornell_box();
        aspect_ratio = 1;
        image_width = 600;
        samples_per_pixel = 200;
        background = color(0, 0, 0);
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
        break;
    default:
        exit(1);
    }

    bvh_node bvh_world(world, 0, 1.0);

    // Camera
    vec3 vup(0, 1, 0);
    double dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperature, dist_to_focus, 0.0, 1.0);

    uint8_t* image_data = new uint8_t[image_width * image_height * 3];
    int image_index = 0;

    // Render
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
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_data_color(image_data, &image_index, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone scanning.\n";

    std::cerr << "Saving output...\n";

    stbi_write_png("image.png", image_width, image_height, 3, image_data, sizeof(uint8_t) * image_width * 3);

    std::cerr << "Done saving output.\n";
}