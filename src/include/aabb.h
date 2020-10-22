#pragma once
#ifndef AABB_H
#define AABB_H

#include "common.h"

class aabb {
public:
    aabb() {}
    aabb(const point3& a, const point3& b) : _min(a), _max(b) {}

    point3 min() const { return _min; }
    point3 max() const { return _max; }

    bool hit(const ray& r, double tmin, double tmax) const {
        for (int a = 0; a < 3; a++) {
            auto invB = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invB;
            auto t1 = (max()[a] - r.origin()[a]) * invB;
            
            if (invB < 0.0f)
                std::swap(t0, t1);

            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin)
                return false;
        }
        return true;
    }

public:
    point3 _min;
    point3 _max;
};

aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(
        fmin(box0.min().x(), box1.min().x()),
        fmin(box0.min().y(), box1.min().y()),
        fmin(box0.min().z(), box1.min().z())
    );
    point3 big(
        fmax(box0.min().x(), box1.min().x()),
        fmax(box0.min().y(), box1.min().y()),
        fmax(box0.min().z(), box1.min().z())
    );
    return aabb(small, big);
}

#endif
