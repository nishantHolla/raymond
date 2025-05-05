#include <iostream>

#include "vector3.h"

// Image

const int IMAGE_WIDTH = 256;
const int IMAGE_HEIGHT = 256;

int main(void) {

  // Render

  std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

  for (int row = 0; row < IMAGE_HEIGHT; row++) {
    std::clog << "\rScannlines remaining: " << (IMAGE_HEIGHT - row) << ' ' << std::flush;

    for (int col = 0; col < IMAGE_WIDTH; col++) {
      const double r = double(col) / (IMAGE_WIDTH - 1);
      const double g = double(row) / (IMAGE_HEIGHT - 1);
      const double b = 0.0;

      const int ir = int(r * 255.999);
      const int ig = int(g * 255.999);
      const int ib = int(b * 255.999);

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }
  std::clog << "\rDone                            \n";

  return 0;
}
