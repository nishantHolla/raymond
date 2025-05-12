#ifndef SCENE_H_
#define SCENE_H_

#include <string>
#include <unordered_map>
#include "parser.h"
#include "camera.h"
#include "entity.h"
#include "entity_list.h"
#include "texture.h"

class Scene {
  public:
    Scene(const std::string& scene_file_path) :
      parser(scene_file_path) {
        parser.parse_camera(camera);
        parser.parse_textures(texture_map);
        std::clog << texture_map.size() << "\n";
    }

    void render(const std::string& output_file_path) {
      camera.render(world, output_file_path);
    }

  private:
    Parser parser;
    Camera camera;
    EntityList world;
    std::unordered_map<std::string, shared_ptr<Texture>> texture_map;
};

#endif //!SCENE_H_
