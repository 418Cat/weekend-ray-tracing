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

    auto material_ground = make_shared<lambertian>(color(0.6, 0.6, 0.0));
    auto material_center = make_shared<lambertian>(color(0.9, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_right  = make_shared<metal>(color(0.3, 0.3, 0.8), .0);
    auto material_right_2  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));

    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.5),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-4.0,    1.0, -2.5),   2.5, material_right));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right_2));
    world.add(make_shared<sphere>(point3(3., .7, -10), 4., material_center));

    camera cam;

    cam.ASPECT_RATIO = 16.0 / 9.0;
    cam.IMAGE_WIDTH = 800;
    cam.SAMPLE_PER_PIX = 100;
    cam.MAX_RAYS_DEPTH = 30;

    cam.render(world);

    return 0;
}