#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double deg_to_rad(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "vec3.h"
#include "ray.h"

// Common Colors
const color white = color(1, 1, 1);
const color black = color(0, 0, 0);
const color sky_blue = color(0.5, 0.7, 1.0);


#endif // !COMMON_H

