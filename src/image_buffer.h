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

// ==============================
// ImageBuffer class
// ==============================

class ImageBuffer {
  public:
    /*
     * Constructor for the image buffer with the given width and height of the image
     */
    ImageBuffer(int width, int height) :
      width(width),
      height(height) {
        buffer = new Color[width * height * 3];
      }

    /*
     * Get color refrence to given row and column of the pixel in the buffer
     */
    Color& get(int row, int col) {
      return buffer[row * width + col];
    }

    /*
     * Write the current image buffer to the given file path using stbi
     */
    void write_to_file(const std::string& file_path, const std::string& extension) {
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

      if (extension == ".jpg" || extension == ".jpeg") {
        stbi_write_jpg(file_path.c_str(), width, height, 3, image, 100);
      }
      else if (extension == ".png") {
        stbi_write_png(file_path.c_str(), width, height, 3, image, width * 3);
      }

      delete[] image;
    }

    /*
     * Free the image buffer
     */
    ~ImageBuffer() {
      delete[] buffer;
    }

  private:
    int width;       // width of the image
    int height;      // height of the image
    Color *buffer;   // buffer to hold the color values of the pixels
};

#endif //!IMAGE_BUFFER_H_
