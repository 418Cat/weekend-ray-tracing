#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera
{
    public:
              double    ASPECT_RATIO    = 16./9.;
              int       IMAGE_WIDTH     = 300;
              int       SAMPLE_PER_PIX  = 20;
              int       MAX_RAYS_DEPTH  = 10;
              double    VERTICAL_FOV    = 90.;

        void render(const hittable& world)
        {
            /**
             * Compute values dependent on
             * variables set by user
             */
            initialize();

            // PPM File header
            std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

            for(float y = 0.; y < IMAGE_HEIGHT; y++)
            {
                for(float x = 0.; x < IMAGE_WIDTH; x++)
                {
                    // Center point of current pixel
                    point3 pixel_center = pixel00_loc
                                        + x*pixel_delta_u
                                        + y*pixel_delta_v;

                    color anti_aliased_color;

                    for(int i = 0; i < SAMPLE_PER_PIX; i++)
                    {
                        // Add color computed in said point
                        anti_aliased_color += ray_color(get_ray(x, y), MAX_RAYS_DEPTH, world);
                    }
                    /**
                     * Multiply by the inverse of samples per pixels
                     * to get the average of all colors in [0, 1[
                     */
                    write_color(std::cout, anti_aliased_color * INVERSE_SAMPLES);
                }
                print_progress(30, y / IMAGE_HEIGHT);
            }

            std::clog << std::endl;
        }
    
    private:
              int       IMAGE_HEIGHT;

        const double    FOCAL_LENGTH    = 1.0;
              double    VIEWPORT_HEIGHT;
              double    VIEWPORT_WIDTH;
        const point3    CAMERA_CENTER   = vec3(0., 0., 0.);

              vec3      viewport_u;
              vec3      viewport_v;
              vec3      pixel_delta_u;
              vec3      pixel_delta_v;

              vec3      viewport_upper_left;
              vec3      pixel00_loc;

              double    INVERSE_SAMPLES;

        void initialize()
        {
            IMAGE_HEIGHT    = int(double(IMAGE_WIDTH) / double(ASPECT_RATIO)); // Height divided by aspect ratio

            VIEWPORT_HEIGHT = 2.* tan(degrees_to_radians(VERTICAL_FOV) / 2.) * FOCAL_LENGTH; 

            /**
             * Get the real aspect ratio to avoid rounding
             * problems for the ideal aspect ratio
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

            INVERSE_SAMPLES = 1./SAMPLE_PER_PIX;
        }

        /**
         * Ray with random sub-pixel offset
         * from the pixel center point
         */
        ray get_ray(int x, int y)
        {
            vec3 offset = sample_square();

            point3 pixel_point = pixel00_loc                         +
                                 ((x + offset.x()) * pixel_delta_u   +
                                  (y + offset.y()) * pixel_delta_v);
            
            return ray(CAMERA_CENTER, pixel_point-CAMERA_CENTER);
        }

        // Random point in the [-0.5, -0.5] <-> [0.5, 0.5] unit square
        vec3 sample_square() const
        {
            return vec3(random_double()-.5, random_double()-.5, 0);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const
        {
            if(depth <= 0) return color(0, 0, 0);


            hit_record rec;

            // If anything is hit, return a color
            // depending on the hit normal
            if(world.hit(r, interval(0.001, infinity), rec))
            {
                ray scattered;
                color attenuation;

                if(rec.mat->scatter(r, rec, attenuation, scattered))
                {
                    return attenuation * ray_color(scattered, depth-1, world);
                }
                return color(0, 0, 0);
            }

            // Else, draw the sky
            vec3 dir = unit(r.direction());
            double a = 0.5*dir.y() + 1.;

            // Lerp from white to rgb(0.5, 0.7, 1)
            color c = (1. - a)*vec3(1., 1., 1.) + a*vec3(.5, .7, 1);

            return c;
        }
};

#endif