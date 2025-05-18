#ifndef BVH_H_
#define BVH_H_

#include <cstdlib>
#include <vector>
#include <algorithm>

#include "raymond.h"
#include "ray.h"
#include "interval.h"
#include "aabb.h"
#include "entity.h"
#include "entity_list.h"

// ==============================
// BVH_Node class
// (derived from Entity class)
// ==============================

class BVH_Node : public Entity {
  public:

    /*
     * Constructs the BVH node and encapsulates all the entities provided in the entity list.
     */
    BVH_Node(EntityList el) :
      BVH_Node(el.list, 0, el.list.size()) {
    }

    /*
     * Constructs the BVH node from the list of entities from start index to end index of the list.
     */
    BVH_Node(std::vector<shared_ptr<Entity>>& entities, size_t start, size_t end) {
      // Start with an empty bounding box
      bound_box = Aabb::empty;

      // Add all the entities
      for (size_t ei = start; ei < end; ei++) {
        bound_box = Aabb(bound_box, entities[ei]->bounding_box());
      }
      // Get the longest axis
      int axis = bounding_box().longest_axis();

      // Check which comparing function to use based in the longest axis
      auto comparator = (axis == 0) ? box_x_compare
                      : (axis == 1) ? box_y_compare
                                    : box_z_compare;

      size_t entity_span = end - start;

      // if only one element in list ad it to both children
      if (entity_span == 1) {
        left = right = entities[start];
      }
      // divide two entities among children
      else if (entity_span == 2) {
        left = entities[start];
        right = entities[start + 1];
      }
      // sort list and then divide it among children
      else {
        std::sort(std::begin(entities) + start, std::begin(entities) + end, comparator);
        size_t mid = start + entity_span / 2;
        left = make_shared<BVH_Node>(entities, start, mid);
        right = make_shared<BVH_Node>(entities, mid, end);
      }
    }

    /*
     * Returns true if the given ray hits the bounding box of the current BVH node in the given time
     * interval.
     */
    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
      if (!bound_box.hit(r, ray_t)) {
        return false;
      }

      bool hit_left = left->hit(r, ray_t, rec);
      bool hit_right = right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

      return hit_left || hit_right;
    }

    /*
     * Returns the bounding box of the current BVH node
     */
    Aabb bounding_box() const override {
      return bound_box;
    }

    /*
     * Returns true if the given index of interval of bounding box a comes before the same index
     * interval of bounding box b.
     */
    static bool box_compare(
        const shared_ptr<Entity> a, const shared_ptr<Entity> b, int axis_index) {
      Interval a_axis_interval = a->bounding_box().axis_interval(axis_index);
      Interval b_axis_interval = b->bounding_box().axis_interval(axis_index);
      return a_axis_interval.min < b_axis_interval.min;
    }

    /*
     * Returns true if the x interval of bouding box a comes before x interval of bounding box b.
     */
    static bool box_x_compare(const shared_ptr<Entity> a, const shared_ptr<Entity> b) {
      return box_compare(a, b, 0);
    }

    /*
     * Returns true if the y interval of bouding box a comes before y interval of bounding box b.
     */
    static bool box_y_compare(const shared_ptr<Entity> a, const shared_ptr<Entity> b) {
      return box_compare(a, b, 1);
    }

    /*
     * Returns true if the z interval of bouding box a comes before z interval of bounding box b.
     */
    static bool box_z_compare(const shared_ptr<Entity> a, const shared_ptr<Entity> b) {
      return box_compare(a, b, 2);
    }

  private:
    shared_ptr<Entity> left;   // left child of the current BVH node
    shared_ptr<Entity> right;  // right child of the current BVH node
    Aabb bound_box;            // the bounding box of the current BVH node
};

#endif //!BVH_H_
