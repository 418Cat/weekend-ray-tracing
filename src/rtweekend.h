#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * PI/180.;
}

void print_progress(int bar_length, double progress_ratio)
{
    std::clog   << "\r[" << std::string(progress_ratio*bar_length, '=') << ">" // Print the arrow
                << std::string(bar_length - progress_ratio*bar_length, ' ') << "]" // Print the spaces
                << progress_ratio*100 << "%"; //Print the % done
}


// Common headers
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif