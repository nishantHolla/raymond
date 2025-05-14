#ifndef COLOR_H_
#define COLOR_H_

#include "interval.h"
#include "vector3.h"

// ==============================
// Color class
// ==============================

class Color {
  public:
    double e[3]; // R, G, B values of the color in [0, 1] range.

    /*
     * Constructs the color to be black if no arguments are passed.
     */
    Color() :
      e{0, 0, 0} {
      }

    /*
     * Constructs the color with given values of red, green and blue.
     */
    Color(double r, double g, double b) :
      e{r, g, b} {
      }

    /*
     * Returns the red value of the current color.
     */
    double r() const {
      return e[0];
    }

    /*
     * Returns the green value of the current color.
     */
    double g() const {
      return e[1];
    }

    /*
     * Returns the blue value of the current color.
     */
    double b() const {
      return e[2];
    }


    /*
     * Operator overload of [] to return individual values of the current color.
     * Throws runtime_error if the accessed index is out of bound.
     */
    double operator[](int i) const {
      if (i < 0 || i > 2) {
        throw std::runtime_error("Color index out of bound");
      }

      return e[i];
    }

    /*
     * Operator overload of [] to return refernece to individual values of the current color.
     * Throws runtime_error if the accessed index is out of bound.
     */
    double& operator[](int i) {
      if (i < 0 || i > 2) {
        throw std::runtime_error("Color index out of bound");
      }

      return e[i];
    }

    /*
     * Operator overload of += to add a color to the current color.
     */
    Color& operator+=(const Color& v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];

      return *this;
    }

    /*
     * Operator overload of *= to the current color by a scalar value.
     */
    Color& operator*=(double t) {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;

      return *this;
    }

    /*
     * Operator overload of /= to divide the current color by a scalar value.
     */
    Color& operator/=(double t) {
      return *this *= (1/t);
    }

};

// ==============================
// Color utility functions
// ==============================

/*
 * Operator overload of << of ostream to print the components of the given color.
 */
inline std::ostream& operator<<(std::ostream& out, const Color& v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

/*
 * Operator overload of + to add two colors.
 */
inline Color operator+(const Color& u, const Color& v) {
  return Color(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

/*
 * Operator overload of - to subtract two colors.
 */
inline Color operator-(const Color& u, const Color& v) {
  return Color(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

/*
 * Opeartor overload of * to multiply two colors.
 */
inline Color operator*(const Color& u, const Color& v) {
  return Color(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

/*
 * Opeartor overload of * to multiply a color by a scalar.
 */
inline Color operator*(double t, const Color& v) {
  return Color(t * v.e[0], t * v.e[1], t * v.e[2]);
}

/*
 * Opeartor overload of * to multiply a color by a scalar.
 */
inline Color operator*(const Color& v, double t) {
  return t * v;
}

/*
 * Opeartor overload of * to divide a color by a scalar.
 */
inline Color operator/(const Color& v, double t) {
  return (1/t) * v;
}

/*
 * Converts the component of color in linear space to gamma space.
 */
inline double linear_to_gamma(double linear_component) {
  if (linear_component > 0) {
    return std::sqrt(linear_component);
  }

  return 0;
}

/*
 * Convert Color whose r, g, b values are from [0, 1] to pixel values which are from [0, 255]
 */
Vector3 color_to_pixel(const Color& color) {
  const double r = color.r();
  const double g = color.g();
  const double b = color.b();

  const double gamma_r = linear_to_gamma(r);
  const double gamma_g = linear_to_gamma(g);
  const double gamma_b = linear_to_gamma(b);

  static const Interval intensity(0.000, 0.999);
  const int ir = int(256 * intensity.clamp(gamma_r));
  const int ig = int(256 * intensity.clamp(gamma_g));
  const int ib = int(256 * intensity.clamp(gamma_b));

  return Vector3(ir, ig, ib);
}

/*
 * Writes the components of given color to the given output stream after converting to gamma space.
 */
void write_color(std::ostream& out, const Color& color) {
  Vector3 pixel = color_to_pixel(color);
  out << int(pixel[0]) << ' ' << int(pixel[1]) << ' ' << int(pixel[2]) << '\n';
}

// ==============================
// Colors
// ==============================

const Color COLOR_PURE_RED = Color(1.0, 0.0, 0.0);
const Color COLOR_PURE_GREEN = Color(0.0, 1.0, 0.0);
const Color COLOR_PURE_BLUE = Color(0.0, 0.0, 1.0);
const Color COLOR_BLUE = Color(0.5, 0.7, 1.0);
const Color COLOR_DARK_BLUE = Color(0.03, 0.03, 0.1);
const Color COLOR_WHITE = Color(1.0, 1.0, 1.0);
const Color COLOR_BLACK = Color(0.0, 0.0, 0.0);

#endif //!COLOR_H_
