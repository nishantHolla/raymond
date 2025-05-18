#ifndef ENTITY_LIST_H_
#define ENTITY_LIST_H_

#include <vector>

#include "raymond.h"
#include "ray.h"
#include "interval.h"
#include "aabb.h"
#include "entity.h"

// ==============================
// Entity class
// (derived from Entity class)
// ==============================

class EntityList: public Entity {
  public:
    std::vector<shared_ptr<Entity>> list; // List of entity objects

    /*
     * Constructs the entity list object with empty list.
     */
    EntityList() {
    }

    /*
     * Constructs the entity list object with the given entity as initial object in the list.
     */
    EntityList(shared_ptr<Entity> entity) {
      add(entity);
    }

    /*
     * Removes all entities in the list.
     */
    void clear() {
      list.clear();
    }

    /*
     * Adds the given entity to the list.
     */
    void add(shared_ptr<Entity> entity) {
      list.push_back(entity);
      bound_box = Aabb(bound_box, entity->bounding_box());
    }

    /*
     * Loops through all entities in the list and calls its hit function.
     * Records the hit with the nearest distance for the given ray at the given interval.
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
      HitRecord temp_record;
      bool hit_anything = false;
      double closest_so_far = ray_t.max;

      for (const auto& e : list) {
        if (e->hit(r, Interval(ray_t.min, closest_so_far), temp_record)) {
          hit_anything = true;
          closest_so_far = temp_record.t;
          rec = temp_record;
        }
      }

      return hit_anything;
    }

    /*
     * Returns the bounding box of the list of entities.
     */
    Aabb bounding_box() const override {
      return bound_box;
    }

  private:
    Aabb bound_box;  // bounding box of the list of entities
};

#endif //!ENTITY_LIST_H_
