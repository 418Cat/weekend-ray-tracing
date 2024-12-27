#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

class hittable_list : public hittable
{

    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> obj) {add(obj);}

        void clear()
        {
            objects.clear();
        }

        void add(shared_ptr<hittable> h)
        {
            objects.push_back(h);
        }

        bool hit(const ray& r, interval ray_interval, hit_record& h_r) const override
        {
            hit_record temp_rec;
            bool hit_anything = false;
            double closest_so_far = ray_interval.max;

            for(const auto& obj : objects)
            {
                /**
                 * Using closest_so_far as t_max
                 * so if the current object is
                 * further, less computations will
                 * be done in the hit() function
                 */
                if(obj->hit(r, interval(ray_interval.min, closest_so_far), temp_rec))
                {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    h_r = temp_rec;
                }
            }

            return hit_anything;
        }

};

#endif