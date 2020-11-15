#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "stb_image_implementation.h"
#include "perlin.h"

#include <iostream>

class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() {}
    solid_color(color c) : color_value(c) {}

    solid_color(double red, double green, double blue)
        : solid_color(color(red, green, blue)) {}

    virtual color value(double u, double v, const vec3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

class checker_texture : public texture {
public:
    checker_texture() {}

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : even(_even), odd(_odd) {}

    checker_texture(color c1, color c2)
        : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

    virtual color value(double u, double v, const point3& p) const override {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class image_texture : public texture {
public:
    const static int bytes_per_pixel = 3;

    image_texture()
        : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(
            filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data)
        {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    virtual color value(double u, double v, const vec3& p) const override {
        // Debug color if we have no tex data
        if (data == nullptr)
            return color(0, 1, 1);

        // Clamp input tex coords to (0,1), (1,0)
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0); // Flip v to bitmap coords

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};

class noise_texture : public texture {
public:
    noise_texture() {};
    noise_texture(double sc, double turb_depth = 1) : scale(sc), turb_depth(turb_depth) {}

    virtual color value(double u, double v, const point3& p) const override {
        return noise.turb(scale * p, turb_depth) * color(1, 1, 1);
    }

public:
    perlin noise;
    double scale, turb_depth;
};

class marble_texture : public noise_texture {
public:
    marble_texture() {}
    marble_texture(double sc, double td, double phase) : noise_texture(sc, td), phase(phase) {}

    virtual color value(double u, double v, const point3& p) const override {
        return 0.5 * (1 + sin(scale * p.z() + phase * noise.turb(p))) * color(1, 1, 1);
    }

public:
    double phase;
};

#endif // !TEXTURE_H

