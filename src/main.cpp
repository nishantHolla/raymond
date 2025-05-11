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


  shared_ptr<Texture> noise_tex = make_shared<NoiseTexture>(4);
  world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(noise_tex)));
  world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(noise_tex)));

  shared_ptr<Material> difflight = make_shared<DiffuseLight>(Color(4, 4, 1));
  world.add(make_shared<Quad>(Point3(4, 2, -2), Vector3(2, 0, 0), Vector3(0, 2, 0), difflight));

  world = EntityList(make_shared<BVH_Node>(world));

  // Setup camera
  Camera camera;

  camera.aspect_ratio = 16.0 / 9.0;
  camera.image_width = 800;
  camera.samples_per_pixel = 100;
  camera.max_depth = 50;
  camera.background = Color(0, 0, 0);

  camera.vfov = 20;
  camera.lookfrom = Point3(26, 3, 6);
  camera.lookat = Point3(0, 2, 0);
  camera.vup = Vector3(0, 1, 0);

  camera.defocus_angle = 0;

  camera.render(world, argv[1]);

  return 0;
}
