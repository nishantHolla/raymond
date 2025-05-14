#ifndef CAMERA_H_
#define CAMERA_H_

#include <omp.h>
#include <atomic>

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "interval.h"
#include "entity.h"
#include "material.h"
#include "image_buffer.h"

// ==============================
// Camera class
// ==============================

class Camera {
  public:
    double aspect_ratio = 1.0;          // aspect ratio of the image produced by the camera
    int image_width = 100;              // width of the image produced by the camera in pixels
    int samples_per_pixel = 10;         // number of samples to calculate per pixel
    int max_depth = 10;                 // maximum number of ray bounces to calculate
    double vfov = 90;                   // vertical field of view
    Point3 lookfrom = Point3(0, 0, 0);  // location of the camera
    Point3 lookat = Point3(0, 0, -1);   // location of the point that the camera is looking at
    Vector3 vup = Vector3(0, 1, 0);     // direction of up for the camera
    Color background;                   // Scene backgound color

    double defocus_angle = 0;           // angle of defocus
    double focus_dist = 10;             // distance of focus from camera

    /*
     * Renders the given list of entities to a P3 file at given file path
     */
    void render(const Entity& world, const std::string& file_path) {
      const std::string extension = file_path.substr(file_path.size() - 4, 4);

      // Initialize private camera attributes based on values of public camera attributes
      initialize();
      ImageBuffer image_buffer(image_width, image_height);
      std::atomic<int> lines_done = 0;

      // setup openmp
      double start = omp_get_wtime();
      omp_set_num_threads(omp_get_max_threads());

      // Trace rays for each pixel
      #pragma omp parallel for schedule(dynamic)
      for (int row = 0; row < image_height; row++) {

        for (int col = 0; col < image_width; col++) {

          Color pixel_color(0, 0, 0);
          for (int sample = 0; sample < samples_per_pixel; sample++) {
            Ray r = get_ray(col, row);
            pixel_color += ray_color(r, max_depth, world);
          }

          image_buffer.get(row, col) = pixel_color * pixel_samples_scale;
        }

        int done = ++lines_done;
        if (done % 10 == 0) {
#pragma omp critical
          {
            std::cerr << "\rProgress: " << done << "/" << image_height
              << " (" << (100 * done / image_height) << "%)" << std::flush;
          }
        }
      }

      // calculate render time
      double end = omp_get_wtime();
      std::clog << "\r[INFO]: Render completed in " << (end - start) << " seconds.\n";

      if (extension == ".ppm") {
        // Open the output file
        std::ofstream image_file(file_path);
        if (!image_file) {
          std::cerr << "Failed to open output image file " << file_path << "\n";
          return;
        }

        // write the image file header
        image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        // write image pixel values
        for (int row = 0; row < image_height; row++) {
          for (int col = 0; col < image_width; col++) {
            write_color(image_file, image_buffer.get(row, col));
          }
        }

        // Close the image file
        image_file.close();
      }
      else {
        image_buffer.write_to_file(file_path, extension);
      }
    }

  private:
    int image_height;              // height of the image produced by the camera
    Point3 center;                 // location of camera center
    Point3 pixel00_loc;            // location of first pixel of the viewport
    double pixel_samples_scale;    // sampling scale factor
    Vector3 pixel_delta_u;         // vector that represents the horizontal change between center of 2 pixels
    Vector3 pixel_delta_v;         // vector that represents the vertical change between center of 2 pixels/ v
    Vector3 u, v, w;               // u and v are unit vectors at center of camera that represents the plane of camera
                                   // w is a unit vector perpendicula to u and v that represents the direction the camera is facing
    Vector3 defocus_disk_u;        // horizontal defocus disk
    Vector3 defocus_disk_v;        // vertical defocus disk

    /*
     * Initialize private camera attributes based on values of public camera attributes before rendering
     */
    void initialize() {
      // Image attributes
      image_height = int(image_width / aspect_ratio);
      image_height = (image_height < 1) ? 1 : image_height;
      pixel_samples_scale = 1.0 / samples_per_pixel;

      // Camera vectors

      center = lookfrom;

      w = unit_vector(lookfrom - lookat);
      u = unit_vector(cross(vup, w));
      v = cross(w, u);

      // Viewport attributes

      double theta = degrees_to_radians(vfov);
      double h = std::tan(theta/2);
      double viewport_height = 2 * h * focus_dist;
      double viewport_width = viewport_height * (double(image_width) / image_height);

      const Vector3 viewport_u = viewport_width * u; // horizontal
      const Vector3 viewport_v = viewport_height * -v; // vertical
      const Point3 viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;

      // Pixel attributes

      pixel_delta_u = viewport_u / image_width;
      pixel_delta_v = viewport_v / image_height;
      pixel00_loc = viewport_upper_left +  0.5 * (pixel_delta_u + pixel_delta_v);

      // Defocus attributes

      double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle/2));
      defocus_disk_u = u * defocus_radius;
      defocus_disk_v = v * defocus_radius;
    }

    /*
     * Gets a random ray for sampling based in given pixel index i and j
     */
    Ray get_ray(int i, int j) const {
      const Vector3 offset = sample_square();
      const Vector3 pixel_sample = pixel00_loc
        + ((i + offset.x()) * pixel_delta_u)
        + ((j + offset.y()) * pixel_delta_v);

      const Point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
      const Vector3 ray_direction = pixel_sample - ray_origin;
      const double ray_time = random_double();

      return Ray(ray_origin, ray_direction, ray_time);
    }

    /*
     * Generates and retruns a random ray offset within the square of -0.5 to 0.5
     */
    Vector3 sample_square() const {
      return Vector3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    /*
     * Generates and returns a random ray for defocusing sample
     */
    Point3 defocus_disk_sample() const {
      Point3 p = random_in_unit_disk();
      return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    /*
     * Calculates the gradient color between a start and end color using linear interpolation
     */
    Color gradient(const Color& start_color, const Color& end_color, double a) const {
      return (1.0 - a) * start_color + a * end_color; // linear interpolation or "lerp" for short
    }

    /*
     * Calculates the color of the ray for a pixel
     */
    Color ray_color(const Ray& r, int depth, const Entity &world) const {
      // If max depth is reached return black color
      if (depth <= 0) {
        return Color(0, 0, 0);
      }

      HitRecord rec;

      // Check if ray hits any entity in the given world
      if (world.hit(r, Interval(0.001, infinity), rec)) {
        Ray scattered;
        Color attenuation;
        Color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        // Calculate the scattered ray based on the material of the entity that has been hit by the ray
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
          // Recursive call the scattered ray
          Color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);
          return color_from_scatter + color_from_emission;
        }

        // If ray is not scattered, return emission color
        return color_from_emission;
      }

      // If nothing is hit, calculate the gradient value for the background
      // Vector3 unit_direction = unit_vector(r.direction());
      // const double a = 0.5 * (unit_direction.y() + 1.0);
      // return gradient(COLOR_DARK_BLUE, COLOR_BLUE, a);

      // Return the background color
      return background;
    }
};

#endif //!CAMERA_H_
