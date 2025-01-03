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
        metal(const color& albedo, double fuzziness) : albedo(albedo), fuzziness(fuzziness) {}

        bool scatter(const ray& r_in, const hit_record& h_r, color& attenuation, ray& scattered)
        const override
        {
            vec3 reflected = unit(reflect(r_in.direction(), h_r.normal));

            reflected += fuzziness*random_unit_vector();

            scattered = ray(h_r.p, reflected);
            attenuation = albedo;

            return dot(h_r.normal, reflected) > 0;
        }
    
    private:
        color albedo;
        double fuzziness;
};

class dielectric: public material
{
    public:
        dielectric(double refraction_index): refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const hit_record& h_r, color& attenuation, ray& scattered)
        const override
        {
            attenuation = color(1, 1, 1);

            // Change the refraction index if the ray enter/exits the material
            double ri = h_r.front_face ? refraction_index : 1./refraction_index;

            vec3 refracted = refract(r_in.direction(), h_r.normal, ri);

            scattered = ray(h_r.p, refracted);

            return true;
        }
    
    private:
        double refraction_index;
};


#endif