#ifndef QUAD_H_
#define QUAD_H_

#include "vector3.h"
#include "ray.h"
#include "interval.h"
#include "entity.h"
#include "entity_list.h"

// ==============================
// Quad class
// (derived from Entitiy class)
// ==============================

class Quad : public Entity {
  public:
    /*
     * Constructs the quad object with the given point to center of the quad and the horizontal and
     * vertical vectors of the quad
     */
    Quad(const Point3& C, const Vector3& u, const Vector3& v, shared_ptr<Material> mat) :
      Q(C - (u/2) - (v/2)),
      u(u),
      v(v),
      mat(mat) {
        Vector3 n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
      }

    /*
     * Sets the bounding box of the quad by calculating the bounding box of its diagonals
     */
    virtual void set_bounding_box() {
      Aabb bound_box_diagonal1 = Aabb(Q, Q + u + v);
      Aabb bound_box_diagonal2 = Aabb(Q + u, Q + v);
      bound_box = Aabb(bound_box_diagonal1, bound_box_diagonal2);
    }

    /*
     * Returns the bounding box of the quad
     */
    Aabb bounding_box() const override {
      return bound_box;
    }

    /*
     * Checks if the given ray hits the quad in the given interval of time and records the hit
     * in the HitRecord.
     * Returns true if the ray hits, else returns false.
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
      double denom = dot(normal, r.direction());

      // Ray is parallel to plane
      if (std::fabs(denom) < 1e-8) {
        return false;
      }

      // Ray does not intersect with the quad
      double t = (D - dot(normal, r.origin())) / denom;
      if (!ray_t.contains(t)) {
        return false;
      }

      // Ray intersects with the plane
      Point3 intersection = r.at(t);
      Vector3 planar_hitpt_vector = intersection - Q;

      Interval unit_interval(0, 1);
      double alpha = dot(w, cross(planar_hitpt_vector, v));
      double beta = dot(w, cross(u, planar_hitpt_vector));

      if (!unit_interval.contains(alpha) || !unit_interval.contains(beta)) {
        return false;
      }

      rec.u = alpha;
      rec.v = beta;
      rec.t = t;
      rec.p = intersection;
      rec.mat = mat;
      rec.set_face_normal(r, normal);

      return true;
    }

  private:
    Point3 Q;                  // bottom left corner of the quad
    Vector3 u;                 // horizontal length of the quad
    Vector3 v;                 // vertical length of the quad
    Vector3 w;
    Vector3 normal;            // normal vector of the quad
    double  D;                 // Ax + By + Cz = D
    shared_ptr<Material> mat;  // material of the quad
    Aabb bound_box;            // bounding box of the quad
};


inline shared_ptr<EntityList> box(const Point3& center, double length, double width, double height, shared_ptr<Material> mat) {
  shared_ptr<EntityList> sides = make_shared<EntityList>();

  Vector3 width_vec = Vector3(width, 0, 0);
  Vector3 height_vec = Vector3(0, height, 0);
  Vector3 length_vec = Vector3(0, 0, length);

  sides->add(make_shared<Quad>(center - length_vec / 2, width_vec, height_vec, mat));
  sides->add(make_shared<Quad>(center + length_vec / 2, width_vec, height_vec, mat));
  sides->add(make_shared<Quad>(center - width_vec / 2, length_vec, height_vec, mat));
  sides->add(make_shared<Quad>(center + width_vec / 2, length_vec, height_vec, mat));
  sides->add(make_shared<Quad>(center - height_vec / 2, width_vec, length_vec, mat));
  sides->add(make_shared<Quad>(center + height_vec / 2, width_vec, length_vec, mat));

  return sides;
}

#endif //!QUAD_H_
