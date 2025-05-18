#ifndef IMAGE_H_
#define IMAGE_H_

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "external/stb_image.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#include <cstdlib>
#include <iostream>
#include <string>

// ==============================
// Image class
// ==============================

class Image {
  public:
    /*
     * Constructs the image object as empty.
     */
    Image() {
    }

    /*
     * Constructs the image object by loading the image at given file path.
     */
    Image(const char *image_filepath) {
      std::string filepath(image_filepath);
      load(filepath);
    }

    /*
     * Destructs the image object by freeing the buffers.
     */
    ~Image() {
      delete[] bdata;
      stbi_image_free(fdata);
    }

    /*
     * Loads the fdata buffer and calculates bdata buffer for given image path using stb.
     * Returns true if successful, else returns false.
     */
    bool load(const std::string& filename) {
      int n = bytes_per_pixel;

      fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
      if (fdata == nullptr) {
        std::clog << "Hello";
        return false;
      };

      bytes_per_scanline = image_width * bytes_per_pixel;
      convert_to_bytes();

      return true;
    }

    /*
     * Returns width of the image loaded. Returns 0 if no image is loaded.
     */
    int width() const {
      return (fdata == nullptr) ? 0 : image_width;
    }

    /*
     * Returns height of the image loaded. Returns 0 if no image is loaded.
     */
    int height() const {
      return (fdata == nullptr) ? 0 : image_height;
    }

    /*
     * Returns pointer to the section of the array containing the pixel data of given x and y coordinate.
     */
    const unsigned char *pixel_data(int x, int y) const {
      static unsigned char magenta[] = {255, 0, 255};
      if (bdata == nullptr) {
        return magenta;
      }

      x = clamp(x, 0, image_width);
      y = clamp(y, 0, image_height);

      return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
    }

  private:
    const int bytes_per_pixel = 3;     // number of bytes per pixel
    float *fdata = nullptr;            // pointer to store floating point data loaded by stb
    unsigned char *bdata = nullptr;    // pointer to store byte data calculated from fdata
    int image_width = 0;               // width of the image loaded
    int image_height = 0;              // height of the image loaded
    int bytes_per_scanline = 0;        // bytes for each line of the image

    /*
     * Clamps the given x value between low and high
     */
    static int clamp(int x, int low, int high) {
      if (x < low) {
        return low;
      }
      else if (x < high) {
        return x;
      }
      else {
        return high - 1;
      }

    }

    /*
     * Maps values from [0, 1] to [0, 255]
     */
    static unsigned char float_to_byte(float value) {
      if (value <= 0.0) {
        return 0;
      }
      else if (1.0 <= value) {
        return 255;
      }
      else {
        return static_cast<unsigned char>(256.0 * value);
      }
    }

    /*
     * Calculates bdata from fdata
     */
    void convert_to_bytes() {
      int total_bytes = image_width * image_height * bytes_per_pixel;
      bdata = new unsigned char[total_bytes];

      unsigned char *bptr = bdata;
      float *fptr = fdata;
      for (int i = 0; i < total_bytes; i++, fptr++, bptr++) {
        *bptr = float_to_byte(*fptr);
      }
    }
};

#endif //!IMAGE_H_
