#ifndef RAYMOND_H_
#define RAYMOND_H_

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// C++ STD Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

// Headers

#include "color.h"
#include "ray.h"
#include "vector3.h"
#include "interval.h"

// Colors

const Color COLOR_BLUE = Color(0.5, 0.7, 1.0);
const Color COLOR_WHITE = Color(1.0, 1.0, 1.0);
const Color COLOR_BLACK = Color(0.0, 0.0, 0.0);

#endif //!RAYMOND_H_
