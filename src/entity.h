#ifndef ENTITY_H
#define ENTITY_H

class Material;

class HitRecord {
  public:
    Point3 p;
    Vector3 normal;
    shared_ptr<Material> mat;
    double t;
    bool front_face;

    void set_face_normal(const Ray& r, const Vector3& outward_normal) {
      front_face = dot(r.direction(), outward_normal) < 0;
      normal = front_face ? outward_normal : -outward_normal;
    }
};

class Entity {
  public:
    virtual ~Entity() = default;
    virtual bool hit(const Ray& r, Interval ray_t, HitRecord &rec) const = 0;
};

#endif //!ENTITY_H
