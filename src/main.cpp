#include <iostream>

#include "vector3.h"
#include "color.h"
#include "ray.h"

// Image

const int IMAGE_WIDTH = 256;
const int IMAGE_HEIGHT = 256;

int main(void) {

  // Render

  std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

  for (int row = 0; row < IMAGE_HEIGHT; row++) {
    std::clog << "\rScannlines remaining: " << (IMAGE_HEIGHT - row) << ' ' << std::flush;

    for (int col = 0; col < IMAGE_WIDTH; col++) {
      const Color pixel_color = Color(
          double(col) / (IMAGE_WIDTH - 1),
          double(row) / (IMAGE_HEIGHT - 1),
          0.0
          );

      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone                            \n";

  return 0;
}
