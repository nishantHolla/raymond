#include <fstream>

#include "raymond.h"
#include "camera.h"
#include "entity.h"
#include "entity_list.h"
#include "material.h"
#include "sphere.h"

int main(int argc, char * argv[]) {
  // Parse arguments

  if (argc != 2) {
    std::cerr << "Usage: raymond <image_output.ppm>\n";
    return 1;
  }

  // Materials

  const shared_ptr<Material> ground_mat = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  const shared_ptr<Material> diff_blue_mat = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  const shared_ptr<Material> clear_metal_mat = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
  const shared_ptr<Material> metal_mat = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);
  const shared_ptr<Material> dielectric_mat = make_shared<Dielectric>(1.50);
  const shared_ptr<Material> air_mat = make_shared<Dielectric>(1/1.50);

  // Setup world

  EntityList world;
  world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5, diff_blue_mat));
  world.add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5, dielectric_mat));
  world.add(make_shared<Sphere>(Point3(1, 0, -1), 0.5, clear_metal_mat));
  world.add(make_shared<Sphere>(Point3(-1, 0, -1), 0.4, air_mat));
  world.add(make_shared<Sphere>(Point3(0, -1000.5, -1), 1000, ground_mat));

  Camera camera;
  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 800;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;
  camera.lookfrom = Point3(-2, 2, 1);
  camera.lookat = Point3(0, 0, -1);
  camera.vup = Vector3(0, 1, 0);
  camera.vfov = 20;
  camera.defocus_angle = 10.0;
  camera.defocus_dist = 3.4;

  camera.render(world, argv[1]);

  return 0;
}
