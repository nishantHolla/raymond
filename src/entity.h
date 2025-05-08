#ifndef ENTITY_H
#define ENTITY_H

#include "aabb.h"

class Material;

// ==============================
// HitRecord class
// ==============================

class HitRecord {
  public:
    Point3 p;                   // Point of hit on the surface that got hit
    Vector3 normal;             // Normal vector at the point of hit with respect to the surface that got hit
    shared_ptr<Material> mat;   // Material of the surface that got hit
    double t;                   // Time unit at which the ray hit the surface
    bool front_face;            // Did the ray hit the front face or the back face of the surface

    /*
     * Sets the value of front_face and normal based on the result of dot product of the ray and the
     * normal.
     */
    void set_face_normal(const Ray& r, const Vector3& outward_normal) {
      front_face = dot(r.direction(), outward_normal) < 0;
      normal = front_face ? outward_normal : -outward_normal;
    }
};

// ==============================
// Entity class
// ==============================

class Entity {
  public:
    virtual ~Entity() = default;

    /*
     * Function that tests if a given ray hits the entity in any time between the given interval.
     * Records the hit results in the given HitRecord.
     * Returns true if it hits, else returns false.
     */
    virtual bool hit(const Ray& r, Interval ray_t, HitRecord &rec) const = 0;

    /*
     * Returns the bounding box of the entity
     */
    virtual Aabb bounding_box() const = 0;
};

#endif //!ENTITY_H
