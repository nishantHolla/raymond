#include <fstream>

#include "raymond.h"
#include "entity.h"
#include "entity_list.h"
#include "sphere.h"

// Colors

const Color COLOR_BLUE = Color(0.5, 0.7, 1.0);
const Color COLOR_WHITE = Color(1.0, 1.0, 1.0);
const Color COLOR_BLACK = Color(0.0, 0.0, 0.0);

Color gradient(const Color& start_color, const Color& end_color, double a) {
  return (1.0 - a) * end_color + a * start_color; // linear interpolation or "lerp" for short
}

Color ray_color(const Ray& r, const Entity& world) {
  HitRecord rec;
  if (world.hit(r, 0, infinity, rec)) {
    const Vector3 n = rec.normal;
    return 0.5 * Color(n.x() + 1, n.y() + 1, n.z() + 1);
  }

  Vector3 unit_direction = unit_vector(r.direction());
  const double a = 0.5 * (unit_direction.y() + 1.0);

  return gradient(COLOR_BLUE, COLOR_WHITE, a);
}

// Image

const double ASPECT_RATIO = 16.0 / 9.0;
const int IMAGE_WIDTH = 1920;
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

int main(int argc, char * argv[]) {
  // Parse arguments

  if (argc != 2) {
    std::cerr << "Usage: raymond <image_output.ppm>\n";
    return 1;
  }

  std::ofstream image_file(argv[1]);
  if (!image_file) {
    std::cerr << "Failed to open output image file " << argv[1] << "\n";
    return 2;
  }

  // Setup world

  EntityList world;
  world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  // Render

  image_file << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

  for (int row = 0; row < IMAGE_HEIGHT; row++) {
    std::clog << "\rScannlines remaining: " << (IMAGE_HEIGHT - row) << ' ' << std::flush;

    for (int col = 0; col < IMAGE_WIDTH; col++) {
      const Point3 pixel_center = pixel00_loc + (col * pixel_delta_u) + (row * pixel_delta_v);
      const Vector3 ray_direction = pixel_center - camera_center;
      const Ray ray(camera_center, ray_direction);
      const Color pixel_color = ray_color(ray, world);

      write_color(image_file, pixel_color);
    }
  }
  std::clog << "\rDone                            \n";
  image_file.close();

  return 0;
}
