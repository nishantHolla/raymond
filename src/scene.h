#ifndef SCENE_H_
#define SCENE_H_

#include <string>
#include <unordered_map>
#include "parser.h"
#include "camera.h"
#include "entity.h"
#include "entity_list.h"
#include "texture.h"
#include "material.h"

class Scene {
  public:
    Scene(const std::string& scene_file_path) :
      parser(scene_file_path) {

        parser.parse_camera(camera);
        std::clog << "[INFO]: Parsed camera settings\n";

        parser.parse_textures(texture_map);
        std::clog << "[INFO]: Parsed " << texture_map.size() << " textures\n";

        parser.parse_materials(material_map, texture_map);
        std::clog << "[INFO]: Parsed " << material_map.size() << " materials\n";

    }

    void render(const std::string& output_file_path) {
      camera.render(world, output_file_path);
    }

  private:
    Parser parser;
    Camera camera;
    EntityList world;
    std::unordered_map<std::string, shared_ptr<Texture>> texture_map;
    std::unordered_map<std::string, shared_ptr<Material>> material_map;
};

#endif //!SCENE_H_
