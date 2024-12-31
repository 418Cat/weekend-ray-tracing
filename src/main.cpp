#include <iostream>

#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main(int argc, char* argv[])
{
    // World
    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    world.add(make_shared<sphere>(point3(-10, 10, -40), 15));

    camera cam;

    cam.ASPECT_RATIO = 16.0 / 9.0;
    cam.IMAGE_WIDTH = 500;
    cam.SAMPLE_PER_PIX = 100;
    cam.MAX_RAYS_DEPTH = 10;

    cam.render(world);

    return 0;
}