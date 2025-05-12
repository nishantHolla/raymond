#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>

#include "external/json.hpp"
#include "vector3.h"
#include "color.h"
#include "camera.h"
#include "texture.h"
#include "material.h"

using json = nlohmann::json;

class Parser {
  public:
    Parser(const std::string& target_file_path) :
      target_file_path(target_file_path) {
        std::ifstream target_input_stream(target_file_path);

        if (!target_input_stream.good()) {
          throw std::runtime_error(target_file_path + ": File not found");
        }

        target_json = json::parse(target_input_stream);
    };

    void parse_camera(Camera& camera) {
      if (!target_json.contains("camera")) {
        throw std::runtime_error(target_file_path + ":camera Path not found");
      }

      const json& section = target_json["camera"];

      camera.aspect_ratio = parse_float(section, "aspect_ratio", "camera.aspect_ratio");
      camera.image_width = parse_number_unsigned(section, "image_width", "camera.image_width");
      camera.samples_per_pixel = parse_number_unsigned(section, "samples_per_pixel", "camera.samples_per_pixel");
      camera.max_depth = parse_number_unsigned(section, "max_depth", "camera.max_depth");
      camera.background = parse_color(section, "background", "camera.backgound");
      camera.vfov = parse_number_unsigned(section, "vfov", "camera.vfov");
      camera.lookfrom = parse_vector3(section, "lookfrom", "camera.lookfrom");
      camera.lookat = parse_vector3(section, "lookat", "camera.lookat");
      camera.vup = parse_vector3(section, "vup", "camera.vup");
      camera.defocus_angle = parse_float(section, "defocus_angle", "camera.defocus_angle");
    }

    void parse_textures(std::unordered_map<std::string, shared_ptr<Texture>>& texture_map) {
      const json& section = target_json["textures"];

      if (section.type() != json::value_t::object) {
        throw std::runtime_error(target_file_path + ":textures Expected to be an object");
      }

      std::queue<std::string> checker_queue;

      for (const auto& [key, value]: section.items()) {
        const std::string type = parse_string(value, "type", "textures." + key + ".type");

        if (type == "SolidColor") {
          Color albedo = parse_color(value, "albedo", "textures." + key + ".albedo");
          texture_map[key] = make_shared<SolidColor>(albedo);
        }
        else if (type == "ImageTexture") {
          const std::string source = parse_string(value, "source", "textures." + key + ".source");
          texture_map[key] = make_shared<ImageTexture>(source.c_str());
        }
        else if (type == "NoiseTexture") {
          double scale = parse_float(value, "scale", "textures." + key + ".scale");
          texture_map[key] = make_shared<NoiseTexture>(scale);
        }
        else if (type == "CheckerTexture") {
          checker_queue.push(key);
        }
        else {
          throw std::runtime_error(target_file_path + ":textures." + key + ".type Invalid type name");
        }
      }

      while (checker_queue.size()) {
        const std::string key = checker_queue.front();
        checker_queue.pop();
        auto value = section[key];

        const std::string odd = parse_string(value, "odd", "textures." + key + ".odd");

        if (texture_map.find(odd) == texture_map.end()) {
          throw std::runtime_error(target_file_path + ":textures." + key + ".odd Could not find texture named " + odd);
        }

        const std::string even = parse_string(value, "even", "textures." + key + ".even");

        if (texture_map.find(even) == texture_map.end()) {
          throw std::runtime_error(target_file_path + ":textures." + key + ".even Could not find texture named " + even);
        }

        double scale = parse_float(value, "scale", "textures." + key + ".scale");
        texture_map[key] = make_shared<CheckerTexture>(scale, texture_map[even], texture_map[odd]);
      }
    }

    void parse_materials(std::unordered_map<std::string, shared_ptr<Material>>& materials_map,
        std::unordered_map<std::string, shared_ptr<Texture>>& texture_map) {
      const json& section = target_json["materials"];

      if (section.type() != json::value_t::object) {
        throw std::runtime_error(target_file_path + ":materials Expected to be an object");
      }

      for (const auto& [key, value]: section.items()) {
        const std::string type = parse_string(value, "type", "materials." + key + ".type");

        if (type == "Lambertian") {
          if (value.contains("color")) {
            Color color = parse_color(value, "color", "materials." + key + ".albedo");
            materials_map[key] = make_shared<Lambertian>(color);
          }
          else if (value.contains("texture")) {
            const std::string texture_name = parse_string(value, "texture", "materials." + key + ".texture");
            if (texture_map.find(texture_name) == texture_map.end()) {
              throw std::runtime_error(target_file_path + ":materials." + key + ".texture Could not find a texture with name " + texture_name );
            }
            materials_map[key] = make_shared<Lambertian>(texture_map[texture_name]);
          }
          else {
            throw std::runtime_error(target_file_path + ":materials." + key + " Expected either color or texture");
          }
        }
        else if (type == "Metal") {
          Color albedo = parse_color(value, "albedo", "materials." + key + ".albedo");
          double fuzz = parse_float(value, "fuzz", "materials." + key + ".fuzz");
          materials_map[key] = make_shared<Metal>(albedo, fuzz);
        }
        else if (type == "Dielectric") {
          double refraction_index = parse_float(value, "refraction_index", "materials." + key + ".refraction_index");
          materials_map[key] = make_shared<Dielectric>(refraction_index);
        }
        else if (type == "DiffuseLight") {
          if (value.contains("color")) {
            Color color = parse_color(value, "color", "materials." + key + ".albedo");
            materials_map[key] = make_shared<DiffuseLight>(color);
          }
          else if (value.contains("texture")) {
            const std::string texture_name = parse_string(value, "texture", "materials." + key + ".texture");
            if (texture_map.find(texture_name) == texture_map.end()) {
              throw std::runtime_error(target_file_path + ":materials." + key + ".texture Could not find a texture with name " + texture_name );
            }
            materials_map[key] = make_shared<DiffuseLight>(texture_map[texture_name]);
          }
          else {
            throw std::runtime_error(target_file_path + ":materials." + key + " Expected either color or texture");
          }
        }
      }
    }

  private:
    const std::string target_file_path;
    json target_json;

    Vector3 parse_vector3(const json& section, const std::string& value, const std::string& path) {
      if (!section.contains(value)) {
        throw std::runtime_error(target_file_path + ":" + path + " Path not found");
      }

      if (section[value].type() != json::value_t::array) {
        throw std::runtime_error(target_file_path + ":" + path + " Expected to be an array");
      }

      Vector3 vector;
      const json& vector_json = section[value];
      for (int i = 0; i < 3; i++) {
        if (vector_json[i].type() != json::value_t::number_float
            && vector_json[i].type() != json::value_t::number_unsigned
            && vector_json[i].type() != json::value_t::number_integer) {
          throw std::runtime_error(target_file_path + ":" + path + " Expected to be a float");
        }

        vector[i] = vector_json[i];
      }

      return vector;
    }

    Color parse_color(const json& section, const std::string& value, const std::string& path) {
      if (!section.contains(value)) {
        throw std::runtime_error(target_file_path + ":" + path + " Path not found");
      }

      if (section[value].type() != json::value_t::array) {
        throw std::runtime_error(target_file_path + ":" + path + " Expected to be an array");
      }

      Color color;
      const json& color_json = section[value];
      for (int i = 0; i < 3; i++) {
        if (color_json[i].type() != json::value_t::number_float &&
            color_json[i].type() != json::value_t::number_unsigned) {
          throw std::runtime_error(target_file_path + ":" + path + " Expected to be a float");
        }

        if (color_json[i] < 0 || color_json[i] > 1) {
          throw std::runtime_error(target_file_path + ":" + path + " Expected to be between 0.0 and 1.0");
        }

        color[i] = color_json[i];
      }

      return color;
    }

    std::string parse_string(const json& section, const std::string& value, const std::string& path) {
      if (!section.contains(value)) {
        throw std::runtime_error(target_file_path + ":" + path + " Path not found");
      }

      if (section[value].type() != json::value_t::string) {
        throw std::runtime_error(target_file_path + ":" + path + " Expected to be string");
      }

      return section[value];
    }

    double parse_float(const json& section, const std::string& value, const std::string& path) {
      if (!section.contains(value)) {
        throw std::runtime_error(target_file_path + ":" + path + " Path not found");
      }

      if (section[value].type() != json::value_t::number_float
          && section[value].type() != json::value_t::number_integer
          && section[value].type() != json::value_t::number_unsigned) {
        throw std::runtime_error(target_file_path + ":" + path + " Expected to be a float");
      }

      return section[value];
    }

    int parse_number_unsigned(const json& section, const std::string& value, const std::string& path) {
      if (!section.contains(value)) {
        throw std::runtime_error(target_file_path + ":" + path + " Path not found");
      }

      if (section[value].type() != json::value_t::number_unsigned) {
        throw std::runtime_error(target_file_path + ":" + path + " Expected to be a positive integer");
      }

      return section[value];
    }
};

#endif //!PARSER_H_
