#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "include/vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_value, int samples_per_pixel)
{
    auto r = pixel_value.x();
    auto g = pixel_value.y();
    auto b = pixel_value.z();

    // TODO: Why the heck are we doing the division here? Shouldn't this be in the mainloop?
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif // !COLOR_H

