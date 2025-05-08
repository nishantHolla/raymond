#ifndef RAY_H_
#define RAY_H_

#include "vector3.h"

// ==============================
// Ray class
// ==============================

class Ray {
  public:
    /*
     * Constructs the ray with zero vector as origin and direction.
     */
    Ray() {
    }

    /*
     * Constructs the ray with the given origin and direction vector.
     */
    Ray(const Point3& origin, const Vector3& direction) :
      orig(origin),
      dir(direction),
      tm (0) {
      }

    /*
     * Constructs the ray with the given origin and direction vector along with the time of the ray.
     */
    Ray(const Point3& origin, const Vector3& direction, double time) :
      orig(origin),
      dir(direction),
      tm (time) {
      }

    /*
     * Returns a const reference to the origin point of the ray.
     */
    const Point3& origin() const {
      return orig;
    }

    /*
     * Returns a const reference to the direction vector of the ray.
     */
    const Vector3& direction() const {
      return dir;
    }

    /*
     * Returns the position of the ray at the given time unit.
     */
    Point3 at(double t) const {
      return orig + t * dir;
    }

    /*
     * Returns the time of the ray
     */
    double time() const {
      return tm;
    }

  private:
    Point3 orig; // origin point of the ray
    Vector3 dir; // direction vector of the ray
    double tm; // time of the ray
};

#endif //!RAY_H_
