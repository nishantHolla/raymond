#ifndef RAYMOND_H_
#define RAYMOND_H_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <iomanip>

// ==============================
// Using statements
// ==============================

using std::make_shared;
using std::shared_ptr;

// ==============================
// Constants
// ==============================

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// ==============================
// Utility functions
// ==============================

/*
 * Conversts the given degrees value to radians.
 */
inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

/*
 * Returns a random double between the range [0, 1).
 */
inline double random_double() {
  return std::rand() / (RAND_MAX + 1.0);
}

/*
 * Returns a random double between the range [min, max).
 */
inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

/*
 * Returns a random int between the range [min, max].
 */
inline int random_int(int min, int max) {
  return int(random_double(min, max+1));
}

/*
 * Displays the progress line to stdout depending on the given current line and total number of lines.
 */
void display_progress(int current, int total) {
  static const int PROGRESS_SIZE = 20;
  if (current == total) {
    std::clog << "\rDone                                                       \n";
    return;
  }

  std::clog << "\rProgress: [";

  int full = (double(current) / total) * PROGRESS_SIZE;
  int empty = PROGRESS_SIZE - full;

  for (int i = 0; i < full; i++) {
    std::clog << '=';
  }
  for (int j = 0; j < empty; j++) {
    std::clog << ' ';
  }
  std::clog << "] "
    << std::setprecision(2) << double(current) / total * 100 << "%       "
    << std::flush;
}

#endif //!RAYMOND_H_
