#include <fstream>

#include "raymond.h"
#include "texture.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"

int main(int argc, char * argv[]) {
  // Parse arguments

  if (argc != 3) {
    std::cerr << "Usage: raymond <scene_input.json> <image_output.ppm>\n";
    return 1;
  }

  // Setup scene

  try {
    Scene scene(argv[1]);
    scene.render(argv[2]);
  }
  catch (const std::runtime_error& e) {
    std::cerr << "[ERROR]: " << e.what() << "\n";
    return 2;
  }

  return 0;
}
