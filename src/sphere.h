#ifndef SPHERE_H_
#define SPHERE_H_

#include "entity.h"

class Sphere : public Entity {
  public:
    Sphere(const Point3& center, double radius, shared_ptr<Material> mat) :
      center(center),
      radius(std::fmax(0, radius)),
      mat(mat) {
      }

    bool hit(const Ray&r, Interval ray_t, HitRecord& rec) const override {
      const Vector3& d = r.direction();
      const Vector3& Q = r.origin();
      const Vector3 oc = (center - Q);

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
      Vector3 outward_normal = (rec.p - center) / radius;
      rec.set_face_normal(r, outward_normal);
      rec.mat = mat;

      return true;
    }

  private:
    Point3 center;
    double radius;
    shared_ptr<Material> mat;
};

#endif //!SPHERE_H_
