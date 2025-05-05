#ifndef ENTITY_LIST_H_
#define ENTITY_LIST_H_

#include "entity.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class EntityList: public Entity {
  public:
    std::vector<shared_ptr<Entity>> list;

    EntityList() {
    }

    EntityList(shared_ptr<Entity> entity) {
      add(entity);
    }

    void clear() {
      list.clear();
    }

    void add(shared_ptr<Entity> entity) {
      add(entity);
    }

    bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override {
      HitRecord temp_record;
      bool hit_anything = false;
      double closest_so_far = ray_tmax;

      for (const Entity& e : list) {
        if (e->hit(r, ray_tmin, closest_so_far, temp_rec)) {
          hit_anything = true;
          closest_so_far = temp_rec.t;
          rec = temp_rec;
        }
      }

      return hit_anything;
    }
};

#endif //!ENTITY_LIST_H_
