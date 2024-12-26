#include <iostream>

#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

void print_progress(int bar_length, double progress_ratio)
{
    std::clog   << "\r[" << std::string(progress_ratio*bar_length, '=') << ">" // Print the arrow
                << std::string(bar_length - progress_ratio*bar_length, ' ') << "]" // Print the spaces
                << progress_ratio*100 << "%"; //Print the % done
}

color ray_color(const ray& r, const hittable& world)
{
    hit_record rec;

    // If anything is hit, return a color
    // depending on the hit normal
    if(world.hit(r, 0, infinity, rec))
    {
        return 0.5 * color(rec.normal + color(1, 1, 1));
    }

    // Else, draw the sky
    vec3 dir = unit(r.direction());
    double a = 0.5*dir.y() + 1.;

    // Lerp from white to rgb(0.5, 0.7, 1)
    color c = (1. - a)*vec3(1., 1., 1.) + a*vec3(.5, .7, 1.);

    return c;
}

int main()
{

    /**
     *      IMAGE
     */
    const double    ASPECT_RATIO    = 16./9.;
    const int       IMAGE_WIDTH     = 400;
    const int       IMAGE_HEIGHT    = int(double(IMAGE_WIDTH) / double(ASPECT_RATIO));

    /**
     *      CAMERA
     */
    const double    FOCAL_LENGTH    = 1.0;
    const double    VIEWPORT_HEIGHT = 2.;
    const double    VIEWPORT_WIDTH  = VIEWPORT_HEIGHT * (double(IMAGE_WIDTH)/IMAGE_HEIGHT);
    const point3    CAMERA_CENTER   = vec3(0., 0., 0.);


    vec3 viewport_u = vec3(VIEWPORT_WIDTH, 0., 0.); // Vector which goes from left of viewport to right
    vec3 viewport_v = vec3(0., -VIEWPORT_HEIGHT, 0.); // Same but for Y, poiting downwards
    vec3 pixel_delta_u = viewport_u / IMAGE_WIDTH; // Size X of one pixel in viewport
    vec3 pixel_delta_v = viewport_v / IMAGE_HEIGHT; // Same but for Y

    /* Camera point
        x= -(focal length)
        y= -(half vector U)
        z= -(half vector V)
    */
    vec3 viewport_upper_left = CAMERA_CENTER - vec3(0., 0., FOCAL_LENGTH)
                                - viewport_u/2 - viewport_v/2;

    // The top left ray origin, moved half a pixel width&height from viewport_upper_left
    vec3 pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);


    // World
    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));


    // Let's go rendering yay

    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    for(float y = 0.; y < IMAGE_HEIGHT; y++)
    {
        print_progress(30, y / IMAGE_HEIGHT);

        for(float x = 0.; x < IMAGE_WIDTH; x++)
        {
            vec3 pixel_center = pixel00_loc
                                + x*pixel_delta_u
                                + y*pixel_delta_v;

            vec3 ray_dir = pixel_center - CAMERA_CENTER;

            ray r = ray(CAMERA_CENTER, ray_dir);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::cout << std::endl;

    return 0;
}