#ifndef IMAGE_BUFFER_H_
#define IMAGE_BUFFER_H_


#include <string>
#include "color.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external/stb_image_write.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

class ImageBuffer {
  public:
    ImageBuffer(int width, int height) :
      width(width),
      height(height) {
        buffer = new Color[width * height * 3];
      }

    Color& get(int row, int col) {
      return buffer[row * width + col];
    }

    void write_to_file(const std::string& file_path) {
      unsigned char* image = new unsigned char[width * height * 3];

      for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
          int idx = (row * width + col) * 3;
          Vector3 pixel = color_to_pixel(buffer[row * width + col]);
          image[idx + 0] = static_cast<unsigned char>(pixel[0]);
          image[idx + 1] = static_cast<unsigned char>(pixel[1]);
          image[idx + 2] = static_cast<unsigned char>(pixel[2]);
        }
      }

      stbi_write_jpg(file_path.c_str(), width, height, 3, image, 100);
      delete[] image;
    }

    ~ImageBuffer() {
      delete[] buffer;
    }

  private:
    int width;
    int height;
    Color *buffer;
};

#endif //!IMAGE_BUFFER_H_
