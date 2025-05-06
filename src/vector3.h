#ifndef VECTOR3_H_
#define VECTOR3_H_

class Vector3 {
  public:
    double e[3];

    Vector3() :
      e{0, 0, 0} {
      }

    Vector3(double x, double y, double z) :
      e{x, y, z} {
      }

    double x() const {
      return e[0];
    }

    double y() const {
      return e[1];
    }

    double z() const {
      return e[2];
    }

    Vector3 operator-() const {
      return Vector3(-e[0], -e[1], -e[2]);
    }

    double operator[](int i) const {
      if (i < 0 || i > 2) {
        throw std::runtime_error("Vector index out of bound");
      }

      return e[i];
    }

    double& operator[](int i) {
      if (i < 0 || i > 2) {
        throw std::runtime_error("Vector index out of bound");
      }

      return e[i];
    }

    Vector3& operator+=(const Vector3& v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];

      return *this;
    }

    Vector3& operator*=(double t) {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;

      return *this;
    }

    Vector3& operator/=(double t) {
      return *this *= (1/t);
    }

    double length() const {
      return std::sqrt(length_squared());
    }

    double length_squared() const {
      return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    bool near_zero() const {
      static double s = 1e-8;
      return (std::fabs(e[0] < s) && std::fabs(e[1] < s) && std::fabs(e[2] < s));
    }

    static Vector3 random() {
      return Vector3(random_double(), random_double(), random_double());
    }

    static Vector3 random(double min, double max) {
      return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

// Point3 is alias for Vector3
using Point3 = Vector3;

// Vector utility functions

inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v) {
  return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v) {
  return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v) {
  return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3& v) {
  return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(const Vector3& v, double t) {
  return t * v;
}

inline Vector3 operator/(const Vector3& v, double t) {
  return (1/t) * v;
}

inline double dot(const Vector3& u, const Vector3& v) {
  return (u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]);
}

inline Vector3 cross(const Vector3& u, const Vector3& v) {
  return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
      u.e[2] * v.e[0] - u.e[0] * v.e[2],
      u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 unit_vector(const Vector3& v) {
  return v / v.length();
}

inline Vector3 random_unit_vector() {
  while (true) {
    Vector3 p = Vector3::random(-1, 1);
    double lensq = p.length_squared();
    if (1e-160 < lensq && lensq <= 1) {
      return p / sqrt(lensq);
    }
  }
}

inline Vector3 random_on_hemisphere(const Vector3& normal) {
  Vector3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0) {
    return on_unit_sphere;
  }
  else {
    return -on_unit_sphere;
  }
}

inline Vector3 reflect(const Vector3& v, const Vector3& n) {
  return v - 2 * dot(v, n) * n;
}

#endif //!VECTOR3_H_
