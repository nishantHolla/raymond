#ifndef VECTOR3_H_
#define VECTOR3_H_

// ==============================
// Vector3 class
// ==============================

class Vector3 {
  public:
    double e[3]; // 3 components of the vector

    /*
     * Constructs the vector with zero vector if no arguments are passed.
     */
    Vector3() :
      e{0, 0, 0} {
      }

    /*
     * Constructs the vector with the passed arguments as its components.
     */
    Vector3(double x, double y, double z) :
      e{x, y, z} {
      }

    /*
     * Returns the X component of the current vector.
     */
    double x() const {
      return e[0];
    }

    /*
     * Returns the Y component of the current vector.
     */
    double y() const {
      return e[1];
    }

    /*
     * Returns the Z component of the current vector.
     */
    double z() const {
      return e[2];
    }

    /*
     * Rotate the vector by given angle in radians
     */
    Vector3 rotate(double angle, int axis) const {
      double cos = std::cos(angle);
      double sin = std::sin(angle);

      if (axis == 0) {
        return Vector3(
            e[0],
            e[1] * cos - e[2] * sin,
            e[1] * sin + e[2] * cos
            );
      }
      else if (axis == 1) {
        return Vector3(
            e[0] * cos + e[2] * sin,
            e[1],
            e[2] * cos - e[0] * sin
            );
      }
      else if (axis == 2) {
        return Vector3(
            e[0] * cos - e[1] * sin,
            e[0] * sin + e[1] * cos,
            e[2]
            );
      }
      else {
        return *this;
      }
    }

    /*
     * Operator overload of - to return a new vector that is the opposite of the current vector.
     */
    Vector3 operator-() const {
      return Vector3(-e[0], -e[1], -e[2]);
    }

    /*
     * Operator overload of [] to return individual components of the current vector.
     * Throws runtime_error if the accessed index is out of bound.
     */
    double operator[](int i) const {
      if (i < 0 || i > 2) {
        throw std::runtime_error("Vector index out of bound");
      }

      return e[i];
    }

    /*
     * Operator overload of [] to return reference to individual components fo the current vector.
     * Throws runtime_error if the accessed index is out of bound.
     */
    double& operator[](int i) {
      if (i < 0 || i > 2) {
        throw std::runtime_error("Vector index out of bound");
      }

      return e[i];
    }

    /*
     * Operator overload of += to add a vector to the current vector.
     */
    Vector3& operator+=(const Vector3& v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];

      return *this;
    }

    /*
     * Operator overload of *= to multiply a scalar to the current vector.
     */
    Vector3& operator*=(double t) {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;

      return *this;
    }

    /*
     * Operator overload of /= to divide the current vector by a scalar.
     */
    Vector3& operator/=(double t) {
      return *this *= (1/t);
    }

    /*
     * Calculates and returns the length of the current vector.
     */
    double length() const {
      return std::sqrt(length_squared());
    }

    /*
     * Calculates and returns the sum of square of components of the current vector.
     */
    double length_squared() const {
      return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    /*
     * Returns true if length of current vector is near zero, else returns false.
     * 1e-8 is taken as the threshold.
     */
    bool near_zero() const {
      static double s = 1e-8;
      return (std::fabs(e[0] < s) && std::fabs(e[1] < s) && std::fabs(e[2] < s));
    }

    /*
     * Returnas a random vector with its components in the range of [0, 1).
     */
    static Vector3 random() {
      return Vector3(random_double(), random_double(), random_double());
    }

    /*
     * Returns a random vector with its components in the range of given [min, max).
     */
    static Vector3 random(double min, double max) {
      return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

// ==============================
// Point3 class
// ==============================

/*
 * Point3 is an alias of Vector3.
 */
using Point3 = Vector3;

// ==============================
// Vector utility functions
// ==============================

/*
 * Operator overload of << of ostream to print the components of the given vector.
 */
inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

/*
 * Operator overload of + to add two vectors.
 */
inline Vector3 operator+(const Vector3& u, const Vector3& v) {
  return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

/*
 * Operator overload of - to subtract two vectors.
 */
inline Vector3 operator-(const Vector3& u, const Vector3& v) {
  return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

/*
 * Opeartor overload of * to multiply the individual components of two vectors.
 */
inline Vector3 operator*(const Vector3& u, const Vector3& v) {
  return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

/*
 * Operator overload of * to multiply a scalar to a vector.
 */
inline Vector3 operator*(double t, const Vector3& v) {
  return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

/*
 * Operator overload of * to multiply a scalar to a vector.
 */
inline Vector3 operator*(const Vector3& v, double t) {
  return t * v;
}

/*
 * Operator overload of / to divide a vector by a scalar.
 */
inline Vector3 operator/(const Vector3& v, double t) {
  return (1/t) * v;
}

/*
 * Computes and returns the dot product of two given vectors.
 */
inline double dot(const Vector3& u, const Vector3& v) {
  return (u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]);
}

/*
 * Computes and returns the cross product of two given vectors.
 */
inline Vector3 cross(const Vector3& u, const Vector3& v) {
  return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
      u.e[2] * v.e[0] - u.e[0] * v.e[2],
      u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

/*
 * Computes and returns the unit vector of the given vector.
 */
inline Vector3 unit_vector(const Vector3& v) {
  return v / v.length();
}

/*
 * Returns a unit vector with a random direction.
 */
inline Vector3 random_unit_vector() {
  while (true) {
    Vector3 p = Vector3::random(-1, 1);
    double lensq = p.length_squared();
    if (1e-160 < lensq && lensq <= 1) {
      return p / sqrt(lensq);
    }
  }
}

/*
 * Returns a unti vector that is in the same hemisphere as the given normal vector.
 */
inline Vector3 random_on_hemisphere(const Vector3& normal) {
  Vector3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0) {
    return on_unit_sphere;
  }
  else {
    return -on_unit_sphere;
  }
}

/*
 * Calculates and returns the reflected vector of a given vector with respect to a normal vector.
 */
inline Vector3 reflect(const Vector3& v, const Vector3& n) {
  return v - 2 * dot(v, n) * n;
}

/*
 * Calculates and returns the refracted vector of a given vector with respoect to a normal vector and
 * the relative refractive index.
 */
inline Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
  double cos_theta = std::fmin(dot(-uv, n), 1.0);
  Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  Vector3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;

  return r_out_perp + r_out_parallel;
}

/*
 * Returns a random unti vector on a 1 unit radius circle
 */
inline Vector3 random_in_unit_disk() {
  while (true) {
    Vector3 p = Vector3(random_double(-1, 1), random_double(-1, 1), 0);
    if (p.length_squared() < 1) {
      return p;
    }
  }
}

#endif //!VECTOR3_H_
