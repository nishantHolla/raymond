#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <fstream>

#include "external/json.hpp"
#include "vector3.h"
#include "color.h"
#include "camera.h"

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
