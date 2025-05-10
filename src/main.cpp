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

  shared_ptr<NoiseTexture> noise_tex = make_shared<NoiseTexture>(4);
  shared_ptr<Lambertian> surface = make_shared<Lambertian>(noise_tex);
  shared_ptr<Sphere> sphere = make_shared<Sphere>(Point3(0, 0, 0), 2, surface);

  world.add(sphere);
  world = EntityList(make_shared<BVH_Node>(world));

  // Setup camera
  Camera camera;

  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 800;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;

  camera.vfov = 20;
  camera.lookfrom = Point3(0, 0, 12);
  camera.lookat = Point3(0, 0, 0);
  camera.vup = Vector3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(world, argv[1]);

  return 0;
}
