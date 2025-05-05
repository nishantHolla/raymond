#ifndef CAMERA_H_
#define CAMERA_H_

#include "entity.h"

class Camera {
  public:
    double aspect_ratio = 1.0;
    int image_width = 100;


    void render(const Entity& world, const std::string& file_path) {
      initialize();

      std::ofstream image_file(file_path);
      if (!image_file) {
        std::cerr << "Failed to open output image file " << file_path << "\n";
        return;
      }

      image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

      for (int row = 0; row < image_height; row++) {
        std::clog << "\rScannlines remaining: " << (image_height - row) << ' ' << std::flush;

        for (int col = 0; col < image_width; col++) {
          const Point3 pixel_center = pixel00_loc + (col * pixel_delta_u) + (row * pixel_delta_v);
          const Vector3 ray_direction = pixel_center - center;
          const Ray ray(center, ray_direction);
          const Color pixel_color = ray_color(ray, world);

          write_color(image_file, pixel_color);
        }
      }
      std::clog << "\rDone                            \n";
      image_file.close();
    }

  private:
    int image_height;
    Point3 center;
    Point3 pixel00_loc;
    Vector3 pixel_delta_u;
    Vector3 pixel_delta_v;

    void initialize() {
      image_height = int(image_width / aspect_ratio);
      image_height = (image_height < 1) ? 1 : image_height;

      center = Point3(0, 0, 0);

      double focal_length = 1.0;
      double viewport_height = 2.0;
      double viewport_width = viewport_height * (double(image_width) / image_height);

      const Vector3 viewport_u = Vector3(viewport_width, 0, 0); // horizontal
      const Vector3 viewport_v = Vector3(0, -viewport_height, 0); // vertical
      const Point3 viewport_upper_left = center - Vector3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

      pixel_delta_u = viewport_u / image_width;
      pixel_delta_v = viewport_v / image_height;
      pixel00_loc = viewport_upper_left +  0.5 * (pixel_delta_u + pixel_delta_v);
    }

    Color gradient(const Color& start_color, const Color& end_color, double a) const {
      return (1.0 - a) * end_color + a * start_color; // linear interpolation or "lerp" for short
    }

    Color ray_color(const Ray& r, const Entity &world) const {
      HitRecord rec;

      if (world.hit(r, Interval(0, infinity), rec)) {
        const Vector3 n = rec.normal;
        return 0.5 * Color(n.x() + 1, n.y() + 1, n.z() + 1);
      }

      Vector3 unit_direction = unit_vector(r.direction());
      const double a = 0.5 * (unit_direction.y() + 1.0);

      return gradient(COLOR_BLUE, COLOR_WHITE, a);

    }
};

#endif //!CAMERA_H_
