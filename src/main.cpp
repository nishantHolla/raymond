#include <iostream>

#include "vector3.h"
#include "color.h"
#include "ray.h"

// Colors

const Color COLOR_BLUE = Color(0.5, 0.7, 1.0);
const Color COLOR_WHITE = Color(1.0, 1.0, 1.0);
const Color COLOR_BLACK = Color(0.0, 0.0, 0.0);

Color gradient(const Color& start_color, const Color& end_color, double a) {
  return (1.0 - a) * end_color + a * start_color; // linear interpolation or "lerp" for short
}

Color ray_color(const Ray& r) {
  Vector3 unit_direction = unit_vector(r.direction());
  const double a = 0.5 * (unit_direction.y() + 1.0);

  return gradient(COLOR_BLUE, COLOR_WHITE, a);
}

// Image

const double ASPECT_RATIO = 16.0 / 9.0;
const int IMAGE_WIDTH = 400;
const int IMAGE_HEIGHT = (int(IMAGE_WIDTH / ASPECT_RATIO) < 1) ? 1 : int(IMAGE_WIDTH / ASPECT_RATIO);

// Camera

const double FOCAL_LENGTH = 1.0;
const Point3 camera_center = Point3(0, 0, 0);

// Viewport vectors

const double VIEWPORT_HEIGHT = 2.0;
const double VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (double(IMAGE_WIDTH) / IMAGE_HEIGHT);
const Vector3 viewport_u = Vector3(VIEWPORT_WIDTH, 0, 0); // horizontal
const Vector3 viewport_v = Vector3(0, -VIEWPORT_HEIGHT, 0); // vertical
const Point3 viewport_upper_left = camera_center - Vector3(0, 0, FOCAL_LENGTH) - viewport_u / 2 - viewport_v / 2;

// Pixel vectors

const Vector3 pixel_delta_u = viewport_u / IMAGE_WIDTH;
const Vector3 pixel_delta_v = viewport_v / IMAGE_HEIGHT;
const Point3 pixel00_loc = viewport_upper_left +  0.5 * (pixel_delta_u + pixel_delta_v);

int main(void) {

  // Render

  std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

  for (int row = 0; row < IMAGE_HEIGHT; row++) {
    std::clog << "\rScannlines remaining: " << (IMAGE_HEIGHT - row) << ' ' << std::flush;

    for (int col = 0; col < IMAGE_WIDTH; col++) {
      const Point3 pixel_center = pixel00_loc + (col * pixel_delta_u) + (row * pixel_delta_v);
      const Vector3 ray_direction = pixel_center - camera_center;
      const Ray ray(camera_center, ray_direction);
      const Color pixel_color = ray_color(ray);

      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone                            \n";

  return 0;
}
