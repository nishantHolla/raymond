#ifndef AABB_H_
#define AABB_H_

#include <algorithm>

#include "vector3.h"
#include "ray.h"
#include "interval.h"

// ==============================
// Aabb class
// ==============================

class Aabb {
  public:
    Interval x, y, z;

    /*
     * Constructs Aabb with empty intervals for x, y and z.
     */
    Aabb() {
    }

    /*
     * Constructs Aabb with given intervals for x, y and z.
     */
    Aabb(const Interval& x, const Interval &y, const Interval& z) :
      x(x),
      y(y),
      z(z) {
        pad_to_mimimums();
    }

    /*
     * Constructs Aabb with two points a and b as the extrema for the thre axes
     */
    Aabb(const Point3& a, const Point3& b) {
      x = (a[0] < b[0] ? Interval(a[0], b[0]) : Interval(b[0], a[0]));
      y = (a[1] < b[1] ? Interval(a[1], b[1]) : Interval(b[1], a[1]));
      z = (a[2] < b[2] ? Interval(a[2], b[2]) : Interval(b[2], a[2]));

      pad_to_mimimums();
    }

    /*
     * Constructs Aabb with two bounding boxes as input
     */
    Aabb(const Aabb& box0, const Aabb& box1) {
      x = Interval(box0.x, box1.x);
      y = Interval(box0.y, box1.y);
      z = Interval(box0.z, box1.z);
    }

    /*
     * Returns the axis interval
     */
    const Interval& axis_interval(int n) const {
      if (n == 1) {
        return y;
      }
      else if (n == 2) {
        return z;
      }
      else {
        return x;
      }
    }

    /*
     * Checks if the given ray hits the bounding box in the given time interval.
     * Returns true if it hits, else returns false.
     */
    bool hit(const Ray& r, Interval ray_t) const {
      const Point3& ray_orig = r.origin();
      const Vector3& ray_dir = r.direction();

      for (int axis = 0; axis < 3; axis++) {
        const Interval& ax = axis_interval(axis);
        const double adinv= 1.0 / ray_dir[axis];

        double t0 = (ax.min - ray_orig[axis]) * adinv;
        double t1 = (ax.max - ray_orig[axis]) * adinv;

        if (t1 < t0) {
          std::swap(t0, t1);
        }

        if (t0 > ray_t.min) {
          ray_t.min = t0;
        }

        if (t1 < ray_t.max) {
          ray_t.max = t1;
        }

        if (ray_t.max <= ray_t.min) {
          return false;
        }

      }

      return true;
    }

    /*
     * Returns the index of the longest axis of the bounding box.
     */
    int longest_axis() const {
      if (x.size() > y.size()) {
        return x.size() > z.size() ? 0 : 2;
      }
      else {
        return y.size() > z.size() ? 1 : 2;
      }
    }

    /*
     * Make sure no side of AABB is narrower than some delta.
     */
    void pad_to_mimimums() {
      double delta = 0.0001;
      if (x.size() < delta) {
        x = x.expand(delta);
      }

      if (y.size() < delta) {
        y = y.expand(delta);
      }

      if (z.size() < delta) {
        z = z.expand(delta);
      }
    }

    static const Aabb empty, universe; // empty bounding box and universe bounding box
};


inline const Aabb Aabb::empty = Aabb(Interval::empty, Interval::empty, Interval::empty);
inline const Aabb Aabb::universe = Aabb(Interval::universe, Interval::universe, Interval::universe);

#endif // !AABB_H_
