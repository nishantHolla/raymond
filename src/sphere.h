#ifndef SPHERE_H_
#define SPHERE_H_

#include <cmath>

#include "raymond.h"
#include "aabb.h"
#include "vector3.h"
#include "ray.h"
#include "interval.h"
#include "entity.h"

// ==============================
// Sphere class
// (derived from Entity class)
// ==============================

class Sphere : public Entity {
  public:
    double rotation = 0.0;

    /*
     * Constructs stationary sphere with the given center, radius and surface material.
     */
    Sphere(const Point3& center, double radius, shared_ptr<Material> mat) :
      center(center, Vector3(0, 0, 0)),
      radius(std::fmax(0, radius)),
      mat(mat) {
        Vector3 rvec = Vector3(radius, radius, radius);
        bound_box = Aabb(center - rvec, center + rvec);
      }

    /*
     * Constructs moving sphere with the given initial center, final center2, radius and surface material.
     */
    Sphere(const Point3& center1, const Point3& center2, double radius, shared_ptr<Material> mat) :
      center(center1, center2 - center1),
      radius(std::fmax(0, radius)),
      mat(mat) {
        Vector3 rvec = Vector3(radius, radius, radius);
        Aabb box1(center.at(0) - rvec, center.at(0) + rvec);
        Aabb box2(center.at(1) - rvec, center.at(1) + rvec);
        bound_box=  Aabb(box1, box2);
      }

    /*
     * Checks if the given ray hits the shpere in the given interval of time and records the hit
     * in the HitRecord.
     * Returns true if the ray hits, else returns false.
     */
    bool hit(const Ray&r, Interval ray_t, HitRecord& rec) const override {
      Point3 current_center = center.at(r.time());

      const Vector3& d = r.direction();
      const Vector3& Q = r.origin();
      const Vector3 oc = (current_center - Q);

      const double a = d.length_squared(); // same as dot(a, a)
      const double h = dot(d, oc); // b = -2h
      const double c = oc.length_squared() - radius * radius; // same as dot(oc, oc) - radius * radius

      const double discriminant = h * h - a * c;
      if (discriminant < 0) {
        return false;
      }

      const double sqrtd = std::sqrt(discriminant);

      // check either of the roots are inside the range

      double root = (h - sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
          return false;
        }
      }

      rec.t = root;
      rec.p = r.at(rec.t);
      Vector3 outward_normal = (rec.p - current_center) / radius;
      rec.set_face_normal(r, outward_normal);
      get_sphere_uv(outward_normal, rec.u, rec.v, rotation);
      rec.mat = mat;

      return true;
    }

    /*
     * Maps a given 3d point in space to a 2d surface and stores the coordinates in u and v.
     */
    static void get_sphere_uv(const Point3& p, double& u, double& v, const double rotation = 0) {
      double theta = std::acos(-p.y());
      double phi = std::atan2(-p.z(), p.x()) + pi;

      phi += rotation;

      if (phi < 0) phi += 2 * pi;
      if (phi >= 2 * pi) phi -= 2 * pi;

      if (theta < 0) theta += 2 * pi;
      if (theta >= 2 * pi) theta -= 2 * pi;

      u = phi / (2 * pi);
      v = theta / pi;
    }

    /*
     * Returns the bounding box of the sphere
     */
    Aabb bounding_box() const override {
      return bound_box;
    }

  private:
    Ray center;                // center of the sphere
    double radius;             // radius of the sphere
    shared_ptr<Material> mat;  // surface material of the sphere
    Aabb bound_box;            // bounding box of the sphere

};

#endif //!SPHERE_H_
