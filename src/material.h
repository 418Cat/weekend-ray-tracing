#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material
{
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const ray& r_in, const hit_record& h_r, color& attenuation, ray& scattered
        ) const
        {
            return false;
        }
};

class lambertian: public material
{
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& h_r, color& attenuation, ray& scattered)
        const override
        {
            vec3 scatter_direction = h_r.normal + random_unit_vector();

            if(scatter_direction.near_zero())
            {
                scatter_direction = h_r.normal;
            }

            scattered = ray(h_r.p, scatter_direction);
            attenuation = albedo;

            return true;
        }

    private:
        color albedo;
};

class metal: public material
{
    public:
        metal(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& h_r, color& attenuation, ray& scattered)
        const override
        {
            vec3 reflected = reflect(r_in.direction(), h_r.normal);
            scattered = ray(h_r.p, reflected);
            attenuation = albedo;

            return true;
        }
    
    private:
        color albedo;
};


#endif