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

    double R = std::cos(PI/4);

    auto material_left = make_shared<lambertian>(color(0., 0., 1.));
    auto material_right= make_shared<lambertian>(color(1., 0., 0.));

    world.add(make_shared<sphere>(point3(-R, 0., -1.), R, material_left));
    world.add(make_shared<sphere>(point3( R, 0., -1.), R, material_right));

    camera cam;

    cam.ASPECT_RATIO = 16.0 / 9.0;
    cam.IMAGE_WIDTH = 400;
    cam.VERTICAL_FOV = 90.;
    cam.SAMPLE_PER_PIX = 100;
    cam.MAX_RAYS_DEPTH = 50;

    cam.VERTICAL_FOV = 90.;

    cam.render(world);

    return 0;
}