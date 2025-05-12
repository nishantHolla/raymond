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
#include "sphere.h"
#include "quad.h"
#include "bvh.h"

using TextureMap = std::unordered_map<std::string, shared_ptr<Texture>>;
using MaterialMap = std::unordered_map<std::string, shared_ptr<Material>>;
using EntityMap = std::unordered_map<std::string, shared_ptr<Entity>>;

// ==============================
// Scene class
// ==============================

class Scene {
  public:
    /*
     * Construct the scene object with the path to the scene file
     */
    Scene(const std::string& scene_file_path) :
      parser(scene_file_path) {

        parser.parse_camera(camera);
        std::clog << "[INFO]: Parsed camera settings\n";

        parser.parse_textures(texture_map);
        std::clog << "[INFO]: Parsed " << texture_map.size() << " textures\n";

        parser.parse_materials(material_map, texture_map);
        std::clog << "[INFO]: Parsed " << material_map.size() << " materials\n";

        parser.parse_entities(entity_map, material_map);
        std::clog << "[INFO]: Parsed " << entity_map.size() << " entities\n";

        for (const auto& [_, value] : entity_map) {
          world.add(value);
        }

        world = EntityList(make_shared<BVH_Node>(world));
    }

    /*
     * Render the scene to an output image file
     */
    void render(const std::string& output_file_path) {
      camera.render(world, output_file_path);
    }

  private:
    Parser parser;               // parser to parse the scene json file
    Camera camera;               // scene's camera
    EntityList world;            // scene's world
    TextureMap texture_map;      // scene's textures
    MaterialMap material_map;    // scene's materials
    EntityMap entity_map;        // scene's entities
};

#endif //!SCENE_H_
