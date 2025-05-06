#ifndef COLOR_H_
#define COLOR_H_

class Color {
  public:
    double e[3];

    Color() :
      e{0, 0, 0} {
      }

    Color(double r, double g, double b) :
      e{r, g, b} {
      }

    double r() const {
      return e[0];
    }

    double g() const {
      return e[1];
    }

    double b() const {
      return e[2];
    }

    Color operator-() const {
      return Color(-e[0], -e[1], -e[2]);
    }

    double operator[](int i) const {
      if (i < 0 || i > 2) {
        throw std::runtime_error("Color index out of bound");
      }

      return e[i];
    }

    double& operator[](int i) {
      if (i < 0 || i > 2) {
        throw std::runtime_error("Color index out of bound");
      }

      return e[i];
    }

    Color& operator+=(const Color& v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];

      return *this;
    }

    Color& operator*=(double t) {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;

      return *this;
    }

    Color& operator/=(double t) {
      return *this *= (1/t);
    }

};

// Color utility functions

inline std::ostream& operator<<(std::ostream& out, const Color& v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Color operator+(const Color& u, const Color& v) {
  return Color(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Color operator-(const Color& u, const Color& v) {
  return Color(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Color operator*(const Color& u, const Color& v) {
  return Color(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Color operator*(double t, const Color& v) {
  return Color(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Color operator*(const Color& v, double t) {
  return t * v;
}

inline Color operator/(const Color& v, double t) {
  return (1/t) * v;
}

inline double linear_to_gamma(double linear_component) {
  if (linear_component > 0) {
    return std::sqrt(linear_component);
  }

  return 0;
}

void write_color(std::ostream& out, const Color& pixel_color) {
  const double r = pixel_color.r();
  const double g = pixel_color.g();
  const double b = pixel_color.b();

  const double gamma_r = linear_to_gamma(r);
  const double gamma_g = linear_to_gamma(g);
  const double gamma_b = linear_to_gamma(b);

  static const Interval intensity(0.000, 0.999);
  const int ir = int(256 * intensity.clamp(gamma_r));
  const int ig = int(256 * intensity.clamp(gamma_g));
  const int ib = int(256 * intensity.clamp(gamma_b));

  out << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif //!COLOR_H_
