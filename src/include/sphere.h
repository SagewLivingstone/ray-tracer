#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public hittable {
public:
    Sphere() {}
    Sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {}

    virtual bool hit(
        const ray& r, double tmin, double tmax, hit_record& rec) const override;

public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};

bool Sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);

        // TODO: Don't always check for normal? Waste of compute if already hit

        // Check the negative root
        auto tmp = (-half_b - root) / a;
        if (tmp < tmax && tmp > tmin) {
            rec.t = tmp;
            rec.p = r.at(tmp);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        // Check the positive root
        tmp = (-half_b + root) / a;
        if (tmp < tmax && tmp > tmin) {
            rec.t = tmp;
            rec.p = r.at(tmp);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        return false;
    }

    return false;
}

#endif
