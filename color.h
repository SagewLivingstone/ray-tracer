#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_value)
{
    out << static_cast<int>(255.999 * pixel_value.x()) << ' '
        << static_cast<int>(255.999 * pixel_value.y()) << ' '
        << static_cast<int>(255.999 * pixel_value.z()) << '\n';
}

#endif // !COLOR_H

