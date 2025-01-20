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

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<metal>(color(0.1, 0.2, 0.5), 0.05);
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.6);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.vertical_fov = 40.;
    cam.sample_per_pix = 50;

    cam.max_ray_depth = 20;
    cam.look_from = point3(-2,2,1);
    cam.look_at   = point3(0,0,-1);
    cam.v_up      = vec3(0,1,0);

    cam.defocus_angle  = 5.0;
    cam.focus_distance = 3.4;

    cam.render(world);

    return 0;
}