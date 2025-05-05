#ifndef ENTITY_LIST_H_
#define ENTITY_LIST_H_

#include "entity.h"

#include <vector>

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
      list.push_back(entity);
    }

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
};

#endif //!ENTITY_LIST_H_
