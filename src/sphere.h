#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
    public:
        sphere(const point3& p, double r) : center(p), radius(std::fmax(0, r)) {};

        // See "./maths/Sphere intersect.pdf"
        bool hit(const ray& r, interval ray_interval, hit_record& h_r) const override
        {
            const vec3 V = center - r.origin();

            const double a = dot(r.direction(), r.direction());
            
            // See "./maths/Sphere intersect simplification.pdf"
            const double h = dot(V, r.direction());
            const double b = -2.*h;

            const double c = dot(V, V) - radius*radius;

            /**
             * discr =   (b²-4ac) / 4
             * Changed to optimise computation
             * of the roots and can be used to
             * cheaply return if discr < 0
             */
            const double discr = h*h - a*c;

            if(discr < 0)
            {
                return false;
            }

            const double sqrt_d = std::sqrt(discr);


            double root = (h - sqrt_d)/a;

            // If first root is outside of range [t_min, t_max]
            if(!ray_interval.surrounds(root))
            {
                // Try second root
                root = (h + sqrt_d)/a;

                if(!ray_interval.surrounds(root))
                {
                    //If both roots are outside
                    return false;
                }
            }

            // Set hit infos
            h_r.t = root;
            h_r.p = r.at(root);
            vec3 outward_normal = (center - h_r.p) / radius;

            h_r.set_face_normal(r, outward_normal);

            return true;
        }


    private:
        point3 center;
        double radius;

};

#endif