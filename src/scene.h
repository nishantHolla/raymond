#ifndef SCENE_H_
#define SCENE_H_

#include <string>
#include "parser.h"
#include "camera.h"
#include "entity.h"
#include "entity_list.h"

class Scene {
  public:
    Scene(const std::string& scene_file_path) :
      parser(scene_file_path) {
        parser.parse_camera(camera);
    }

    void render(const std::string& output_file_path) {
      camera.render(world, output_file_path);
    }

  private:
    Parser parser;
    Camera camera;
    EntityList world;
};

#endif //!SCENE_H_
