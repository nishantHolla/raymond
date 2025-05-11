#include <fstream>

#include "raymond.h"
#include "camera.h"
#include "entity.h"
#include "entity_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "bvh.h"

int main(int argc, char * argv[]) {
  // Parse arguments

  if (argc != 2) {
    std::cerr << "Usage: raymond <image_output.ppm>\n";
    return 1;
  }

  // Setup world

  EntityList world;

  shared_ptr<Material> left = make_shared<Lambertian>(Color(1.0, 0.0, 0.0));
  shared_ptr<Material> right = make_shared<Lambertian>(Color(0.0, 1.0, 0.0));
  shared_ptr<Material> top = make_shared<Lambertian>(Color(0.0, 0.0, 1.0));
  shared_ptr<Material> bottom = make_shared<Lambertian>(Color(1.0, 0.0, 1.0));
  shared_ptr<Material> front = make_shared<Lambertian>(Color(1.0, 1.0, 0.0));

  world.add(make_shared<Quad>(Point3(-2, 0, 2), Vector3(0, 0, -4), Vector3(0, 4, 0), left));
  world.add(make_shared<Quad>(Point3(0, 0, 0), Vector3(4, 0, 0), Vector3(0, 4, 0), front));
  world.add(make_shared<Quad>(Point3(2, 0, 2), Vector3(0, 0, 4), Vector3(0, 4, 0), right));
  world.add(make_shared<Quad>(Point3(0, 2, 2), Vector3(4, 0, 0), Vector3(0, 0, 4), top));
  world.add(make_shared<Quad>(Point3(0, -2, 2), Vector3(4, 0, 0), Vector3(0, 0, -4), bottom));

  world = EntityList(make_shared<BVH_Node>(world));

  // Setup camera
  Camera camera;

  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 800;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;

  camera.vfov = 80;
  camera.lookfrom = Point3(0, 0, 9);
  camera.lookat = Point3(0, 0, 0);
  camera.vup = Vector3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(world, argv[1]);

  return 0;
}
