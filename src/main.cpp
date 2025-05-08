#include <fstream>

#include "raymond.h"
#include "camera.h"
#include "entity.h"
#include "entity_list.h"
#include "material.h"
#include "sphere.h"
#include "bvh.h"

int main(int argc, char * argv[]) {
  // Parse arguments

  if (argc != 2) {
    std::cerr << "Usage: raymond <image_output.ppm>\n";
    return 1;
  }

  // Setup world

  EntityList world;

  shared_ptr<Material> ground_mat = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_mat));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      double choose_mat = random_double();
      Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<Material> sphere_mat;

        if (choose_mat < 0.8) {
          // diffuse
          Color albedo = Color(random_double(), random_double(), random_double());
          sphere_mat = make_shared<Lambertian>(albedo);
          Point3 center2 = center + Vector3(0, random_double(0, 0.5), 0);
          world.add(make_shared<Sphere>(center, center2, 0.2, sphere_mat));
        }
        else if (choose_mat < 0.95) {
          // metal
          Color albedo = Color(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
          double fuzz = random_double(0, 0.5);
          sphere_mat = make_shared<Metal>(albedo, fuzz);
          world.add(make_shared<Sphere>(center, 0.2, sphere_mat));
        }
        else {
          // glass
          sphere_mat = make_shared<Dielectric>(1.5);
          world.add(make_shared<Sphere>(center, 0.2, sphere_mat));
        }

      }
    }
  }

  shared_ptr<Material> material1 = make_shared<Dielectric>(1.5);
  world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  shared_ptr<Material> material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  shared_ptr<Material> material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  world = EntityList(make_shared<BVH_Node>(world));

  // Setup camera
  Camera camera;

  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 800;
  camera.samples_per_pixel = 10;
  camera.max_depth = 10;

  camera.vfov = 20;
  camera.lookfrom = Point3(13, 2, 3);
  camera.lookat = Point3(0, 0, 0);
  camera.vup = Vector3(0, 1, 0);

  camera.defocus_angle = 0.6;
  camera.focus_dist = 10.0;

  camera.render(world, argv[1]);

  return 0;
}
