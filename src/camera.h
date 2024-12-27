#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera
{
    public:
              double    ASPECT_RATIO    = 16./9.;
              int       IMAGE_WIDTH     = 300;

        void render(const hittable& world)
        {
            initialize();

            std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

            for(float y = 0.; y < IMAGE_HEIGHT; y++)
            {
                print_progress(30, y / IMAGE_HEIGHT);

                for(float x = 0.; x < IMAGE_WIDTH; x++)
                {
                    point3 pixel_center = pixel00_loc
                                        + x*pixel_delta_u
                                        + y*pixel_delta_v;


                    /*
                     * POC to get anti-aliasing to work without
                     * reading the chapter on it from the book;
                     * Taking 4 different samples per pixel and
                     * averaging the color into one
                     */
                    point3 pix00 = pixel_center - pixel_delta_u/4 - pixel_delta_v/4;
                    point3 pix10 = pixel_center + pixel_delta_u/4 - pixel_delta_v/4;
                    point3 pix11 = pixel_center + pixel_delta_u/4 + pixel_delta_v/4;
                    point3 pix01 = pixel_center - pixel_delta_u/4 + pixel_delta_v/4;

                    color anti_aliased_color = (
                        ray_color(ray(CAMERA_CENTER, pix00 - CAMERA_CENTER), world) +
                        ray_color(ray(CAMERA_CENTER, pix10 - CAMERA_CENTER), world) +
                        ray_color(ray(CAMERA_CENTER, pix11 - CAMERA_CENTER), world) +
                        ray_color(ray(CAMERA_CENTER, pix01 - CAMERA_CENTER), world)) / 4;
                    
                    write_color(std::cout, anti_aliased_color);
                        

                    /*vec3 ray_dir = pixel_center - CAMERA_CENTER;
                    ray r = ray(CAMERA_CENTER, ray_dir);
                    color pixel_color = ray_color(r, world);
                    write_color(std::cout, pixel_color);*/
                }
            }

            std::cout << std::endl;
        }
    
    private:
              int       IMAGE_HEIGHT;

        const double    FOCAL_LENGTH    = 1.0;
        const double    VIEWPORT_HEIGHT = 2.;
              double    VIEWPORT_WIDTH;
        const point3    CAMERA_CENTER   = vec3(0., 0., 0.);

              vec3      viewport_u;
              vec3      viewport_v;
              vec3      pixel_delta_u;
              vec3      pixel_delta_v;

              vec3      viewport_upper_left;
              vec3      pixel00_loc;

        void initialize()
        {

            /**
             *      IMAGE
             */
            
            IMAGE_HEIGHT    = int(double(IMAGE_WIDTH) / double(ASPECT_RATIO));

            /**
             *      CAMERA
             */
            
            VIEWPORT_WIDTH  = VIEWPORT_HEIGHT * (double(IMAGE_WIDTH)/IMAGE_HEIGHT);
            


            viewport_u = vec3(VIEWPORT_WIDTH, 0., 0.); // Vector which goes from left of viewport to right
            viewport_v = vec3(0., -VIEWPORT_HEIGHT, 0.); // Same but for Y, poiting downwards
            pixel_delta_u = viewport_u / IMAGE_WIDTH; // Size X of one pixel in viewport
            pixel_delta_v = viewport_v / IMAGE_HEIGHT; // Same but for Y

            /* Camera point
                x= -(focal length)
                y= -(half vector U)
                z= -(half vector V)
            */
            viewport_upper_left = CAMERA_CENTER - vec3(0., 0., FOCAL_LENGTH)
                                        - viewport_u/2 - viewport_v/2;

            // The top left ray origin, moved half a pixel width&height from viewport_upper_left
            pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);
        }

        color ray_color(const ray& r, const hittable& world) const
        {
            hit_record rec;

            // If anything is hit, return a color
            // depending on the hit normal
            if(world.hit(r, interval(0, infinity), rec))
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
};

#endif