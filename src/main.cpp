#include <fstream>

#include "raymond.h"
#include "texture.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"

int main(int argc, char * argv[]) {
  // Parse arguments

  if (argc != 3) {
    std::cerr << "Usage: raymond <scene_input.json> <image_output.{jpg/png/ppm}>\n";
    return 1;
  }

  // Setup scene and render it

  try {
    Scene scene(argv[1]);
    std::clog << "[INFO]: Preparing to render\n";
    scene.render(argv[2]);
  }
  catch (const std::runtime_error& e) {
    std::cerr << "[ERROR]: " << e.what() << "\n";
    return 2;
  }

  return 0;
}
