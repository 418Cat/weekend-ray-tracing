#include <iostream>

#include "rtweekend.h"

#include "hittable.h"
#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main(int argc, char* argv[])
{
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0., -1000., 0.), 1000., ground_material));

    float square_size = 5.;
    float max_2d_radius = .6; // Max random radius around point x, z in loop
    float min_radius = 0.3;
    float max_radius = 0.55;

    // X and Z coordinates on 2D plane
    for(float x = -square_size/2.; x < square_size/2.; x++)
    {
        for(int z = -square_size/2.; z < square_size/2.; z++)
        {
            // random number dictating which material is chosen
            float material_random = random_double();

            float radius = random_double(min_radius, max_radius) * max_radius;

            point3 center = point3(
                x + random_double()*max_2d_radius,
                radius,
                z + random_double()*max_2d_radius
            );

            shared_ptr<material> mat;

            // Material selection
            // Everything is pretty much random
            if(material_random < 0.7)
            {
                mat = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
            } else
            if(material_random < 0.9)
            {
                mat = make_shared<metal>(color(random_double(), random_double(), random_double()), random_double());
            } else
            {
                mat = make_shared<dielectric>(1. + random_double());
            }

            world.add(make_shared<sphere>(center, radius, mat));
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 300;
    cam.vertical_fov = 20.;
    cam.sample_per_pix = 50;

    cam.max_ray_depth = 5;
    cam.look_from = point3(15, 2.9, 2.6);
    cam.look_at   = point3(-0.3, 0.5, 0.);
    cam.v_up      = vec3(0., 1., 0.);

    cam.defocus_angle  = 1.0;
    cam.focus_distance = (cam.look_from - cam.look_at).length();

    cam.render(world);

    return 0;
}