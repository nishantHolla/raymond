#ifndef RAY_H_
#define RAY_H_

#include "vector3.h"

class Ray {
  public:
    Ray() {
    }

    Ray(const Vector3& origin, const Vector3& direction) :
      orig(origin),
      dir(direction) {
      }

    const Point3& origin() const {
      return orig;
    }

    const Vector3& direction() const {
      return dir;
    }

    Point3 at(double t) const {
      return orig + t * dir;
    }

  private:
    Point3 orig;
    Vector3 dir;
};

#endif //!RAY_H_
