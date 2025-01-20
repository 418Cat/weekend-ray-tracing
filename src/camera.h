#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera
{
    public:
        double    aspect_ratio    = 16./9.;
        int       image_width     = 300;
        int       sample_per_pix  = 20;
        int       max_ray_depth  = 10;
        double    vertical_fov    = 90.;
        vec3      look_at         = vec3(0., 0., -1.);
        vec3      look_from       = vec3(0., 0., 0.);
        vec3      v_up            = vec3(0., 1., 0.);
        double    defocus_angle   = 0;
        double    focus_distance  = 1.;

        void render(const hittable& world)
        {
            /**
             * Compute values dependent on
             * variables set by user
             */
            initialize();

            // PPM File header
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for(float y = 0.; y < image_height; y++)
            {
                for(float x = 0.; x < image_width; x++)
                {
                    // Center point of current pixel
                    point3 pixel_center = pixel00_loc
                                        + x*pixel_delta_u
                                        + y*pixel_delta_v;

                    color anti_aliased_color;

                    for(int i = 0; i < sample_per_pix; i++)
                    {
                        // Add color computed in said point
                        anti_aliased_color += ray_color(get_ray(x, y), max_ray_depth, world);
                    }
                    /**
                     * Multiply by the inverse of samples per pixels
                     * to get the average of all colors in [0, 1[
                     */
                    write_color(std::cout, anti_aliased_color * inverse_samples);
                }
                print_progress(30, y / image_height);
            }

            std::clog << std::endl;
        }
    
    private:
        int       image_height;

        double    focal_length;
        double    viewport_height;
        double    viewport_width;
        point3    camera_center;

        vec3 u, v, w;

        vec3      viewport_u;
        vec3      viewport_v;
        vec3      pixel_delta_u;
        vec3      pixel_delta_v;

        vec3      viewport_upper_left;
        vec3      pixel00_loc;

        double    inverse_samples;

        vec3      defocus_disk_u;
        vec3      defocus_disk_v;

        void initialize()
        {
            image_height = int(double(image_width) / double(aspect_ratio)); // Height divided by aspect ratio

            camera_center = look_from;

            w = look_from - look_at;
            focal_length = w.length();
            w /= focal_length;

            u = unit(cross(v_up, w));
            v = cross(w, u);

            auto theta = degrees_to_radians(vertical_fov);
            auto h = std::tan(theta/2);
            viewport_height = 2.* h * focus_distance;
            /**
             * Get the real aspect ratio to avoid rounding
             * problems for the ideal aspect ratio
             */
            viewport_width  = viewport_height * (double(image_width)/image_height);

            viewport_u = u * viewport_width; // Vector which goes from left of viewport to right
            viewport_v = -v * viewport_height; // Same but for Y, poiting downwards
            pixel_delta_u = viewport_u / image_width; // Size X of one pixel in viewport
            pixel_delta_v = viewport_v / image_height; // Same but for Y

            /* Camera point
                x= -(focal length)
                y= -(half vector U)
                z= -(half vector V)
            */
            viewport_upper_left = camera_center - (w * focal_length)
                                        - viewport_u/2 - viewport_v/2;

            // The top left ray origin, moved half a pixel width&height from viewport_upper_left
            pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

            inverse_samples = 1./sample_per_pix;

            double defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle / 2.));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
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

            point3 ray_origin = defocus_angle <= 0 ? camera_center : defocus_disk_sample();

            return ray(ray_origin, pixel_point-ray_origin);
        }

        // Random point in the [-0.5, -0.5] <-> [0.5, 0.5] unit square
        vec3 sample_square() const
        {
            return vec3(random_double()-.5, random_double()-.5, 0);
        }

        point3 defocus_disk_sample()
        {
            point3 on_unit = random_in_unit_circle();
            return camera_center + defocus_disk_u*on_unit[0] + defocus_disk_v*on_unit[1];
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