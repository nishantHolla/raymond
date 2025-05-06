#ifndef CAMERA_H_
#define CAMERA_H_

#include "entity.h"
#include "material.h"

class Camera {
  public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;
    double vfov = 90; // vertical field of view
    Point3 lookfrom = Point3(0, 0, 0);
    Point3 lookat = Point3(0, 0, -1);
    Vector3 vup = Vector3(0, 1, 0);

    void render(const Entity& world, const std::string& file_path) {
      initialize();

      std::ofstream image_file(file_path);
      if (!image_file) {
        std::cerr << "Failed to open output image file " << file_path << "\n";
        return;
      }

      image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

      for (int row = 0; row < image_height; row++) {
        display_progress(row, image_height - 1);

        for (int col = 0; col < image_width; col++) {

          Color pixel_color(0, 0, 0);
          for (int sample = 0; sample < samples_per_pixel; sample++) {
            Ray r = get_ray(col, row);
            pixel_color += ray_color(r, max_depth, world);
          }

          write_color(image_file, pixel_color * pixel_samples_scale);
        }
      }

      image_file.close();
    }

  private:
    int image_height;
    Point3 center;
    Point3 pixel00_loc;
    double pixel_samples_scale;
    Vector3 pixel_delta_u;
    Vector3 pixel_delta_v;
    Vector3 u, v, w;

    void initialize() {
      image_height = int(image_width / aspect_ratio);
      image_height = (image_height < 1) ? 1 : image_height;
      pixel_samples_scale = 1.0 / samples_per_pixel;

      center = lookfrom;

      double focal_length = (lookfrom - lookat).length();
      double theta = degrees_to_radians(vfov);
      double h = std::tan(theta/2);
      double viewport_height = 2 * h * focal_length;
      double viewport_width = viewport_height * (double(image_width) / image_height);

      w = unit_vector(lookfrom - lookat);
      u = unit_vector(cross(vup, w));
      v = cross(w, u);

      const Vector3 viewport_u = viewport_width * u; // horizontal
      const Vector3 viewport_v = viewport_height * -v; // vertical
      const Point3 viewport_upper_left = center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;

      pixel_delta_u = viewport_u / image_width;
      pixel_delta_v = viewport_v / image_height;
      pixel00_loc = viewport_upper_left +  0.5 * (pixel_delta_u + pixel_delta_v);
    }

    Ray get_ray(int i, int j) const {
      const Vector3 offset = sample_square();
      const Vector3 pixel_sample = pixel00_loc
        + ((i + offset.x()) * pixel_delta_u)
        + ((j + offset.y()) * pixel_delta_v);

      const Point3 ray_origin = center;
      const Vector3 ray_direction = pixel_sample - ray_origin;

      return Ray(ray_origin, ray_direction);
    }

    Vector3 sample_square() const {
      return Vector3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Color gradient(const Color& start_color, const Color& end_color, double a) const {
      return (1.0 - a) * end_color + a * start_color; // linear interpolation or "lerp" for short
    }

    Color ray_color(const Ray& r, int depth, const Entity &world) const {
      if (depth <= 0) {
        return Color(0, 0, 0);
      }

      HitRecord rec;

      if (world.hit(r, Interval(0.001, infinity), rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
          return attenuation * ray_color(scattered, depth-1, world);
        }

        return Color(0, 0, 0);
      }

      Vector3 unit_direction = unit_vector(r.direction());
      const double a = 0.5 * (unit_direction.y() + 1.0);

      return gradient(COLOR_BLUE, COLOR_WHITE, a);

    }
};

#endif //!CAMERA_H_
