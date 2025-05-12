#include <fstream>

#include "raymond.h"
/*#include "material.h"*/
/*#include "sphere.h"*/
/*#include "quad.h"*/
/*#include "bvh.h"*/

int main(int argc, char * argv[]) {
  // Parse arguments

  if (argc != 3) {
    std::cerr << "Usage: raymond <scene_input.json> <image_output.ppm>\n";
    return 1;
  }

  try {
    Scene scene(argv[1]);
    scene.render(argv[2]);
  }
  catch (const std::runtime_error& e) {
    std::cerr << "[ERROR]: " << e.what() << "\n";
    return 2;
  }

  // Setup world

  /*EntityList world;*/


  /*shared_ptr<Texture> noise_tex = make_shared<NoiseTexture>(4);*/
  /*world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(noise_tex)));*/
  /*world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(noise_tex)));*/

  /*shared_ptr<Material> difflight = make_shared<DiffuseLight>(Color(4, 4, 1));*/
  /*world.add(make_shared<Quad>(Point3(4, 2, -2), Vector3(2, 0, 0), Vector3(0, 2, 0), difflight));*/

  /*world = EntityList(make_shared<BVH_Node>(world));*/

  return 0;
}
