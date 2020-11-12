#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "hittable.h"
#include "texture.h"


// Utility Functions

double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

// Material Classes

class material {
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 scatter_direction = rec.normal + random_on_unit_sphere();
        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzziness(f < 1 ? f : 1) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal) + fuzziness * random_on_unit_sphere();
        scattered = ray(rec.p, reflected, r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzziness;
};

class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ref_idx(index_of_refraction) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ref_idx) : ref_idx; // ref index gets flipped if ray direction is backwards

        vec3 unit_direction = unit_vector(r_in.direction());

        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        double reflect_prob = schlick(cos_theta, refraction_ratio);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;

        if (cannot_refract || random_double() < reflect_prob) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected, r_in.time());
            return true;
        }

        vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
        scattered = ray(rec.p, refracted, r_in.time());
        return true;
    }

public:
    double ref_idx;
};

#endif
