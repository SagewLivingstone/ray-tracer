#pragma once
#ifndef AARECT_H
#define AARECT_H

#include "common.h"
#include "hittable.h"

class xy_rect : public hittable {
public:
    xy_rect() {}

    xy_rect(
        double x0, double x1, double y0, double y1, double k, shared_ptr<material> mat
    ) : x0(x0), x1(x1), y0(y0), y1(y1), k(k), mp(mat) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
        // Must have a non-zero width in all dimensions, so we need to pad the z-axis dim
        output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
        return true;
    }

public:
    shared_ptr<material> mp;
    double x0, x1, y0, y1, k;
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k - r.origin().z()) / r.direction().z();

    if (t < t_min || t_max < t)
        return false;

    auto x = r.origin().x() + t * r.direction().x();
    auto y = r.origin().y() + t * r.direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;

    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

class xz_rect : public hittable {
public:
    xz_rect() {}

    xz_rect(
        double x0, double x1, double z0, double z1, double k, shared_ptr<material> mat
    ) : x0(x0), x1(x1), z0(z0), z1(z1), k(k), mp(mat) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
        // Must have a non-zero width in all dimensions, so we need to pad the z-axis dim
        output_box = aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
        return true;
    }

public:
    shared_ptr<material> mp;
    double x0, x1, z0, z1, k;
};

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k - r.origin().y()) / r.direction().y();

    if (t < t_min || t_max < t)
        return false;

    auto x = r.origin().x() + t * r.direction().x();
    auto z = r.origin().z() + t * r.direction().z();

    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;

    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

class yz_rect : public hittable {
public:
    yz_rect() {}

    yz_rect(
        double y0, double y1, double z0, double z1, double k, shared_ptr<material> mat
    ) : y0(y0), y1(y1), z0(z0), z1(z1), k(k), mp(mat) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
        // Must have a non-zero width in all dimensions, so we need to pad the z-axis dim
        output_box = aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
        return true;
    }

public:
    shared_ptr<material> mp;
    double y0, y1, z0, z1, k;
};

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k - r.origin().x()) / r.direction().x();

    if (t < t_min || t_max < t)
        return false;

    auto y = r.origin().y() + t * r.direction().y();
    auto z = r.origin().z() + t * r.direction().z();

    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;

    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

#endif // !AARECT_H

