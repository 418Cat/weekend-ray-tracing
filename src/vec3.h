#ifndef VEC3_H
#define VEC3_H

class vec3
{
    public:
        double e[3];

        vec3():                                 e{0, 0, 0}      {} // Null vector constructor
        vec3(double e0, double e1, double e2):  e{e0, e1, e2}   {} // Standard vector constructor

        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        vec3    operator- ()        const   {return vec3(-e[0], -e[1], -e[2]);} // Return negated vector
        double  operator[](int i)   const   {return e[i];}                      // Read-only array access operator
        double& operator[](int i)           {return e[i];}                      // Pointer to array element

        // Add-equal operator
        vec3& operator+=(const vec3& v)
        {
            e[0] += v[0];
            e[1] += v[1];
            e[2] += v[2];

            return *this;
        }

        // Multiply-equal operator
        vec3& operator*=(const double n)
        {
            e[0] *= n;
            e[1] *= n;
            e[2] *= n;

            return *this;
        }

        // Divide-equal operator
        vec3& operator/=(const double n)
        {   
            return *this *= 1/n;
        }

        double length() const
        {
            return std::sqrt(length_squared());
        }

        double length_squared() const
        {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        // Returns true if the vector is close to 0 in all directions
        bool near_zero()
        {
            double delta = 1e-8;
            return (std::fabs(e[0]) < delta) && (std::fabs(e[1]) < delta) && (std::fabs(e[1]) < delta);
        }

        static vec3 random()
        {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max)
        {
            return vec3(
                random_double(min, max),
                random_double(min, max),
                random_double(min, max)
            );
        }
};

// Point3 is an alias for vec3 but clearer than to use vec3 for both
using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << '(' << v.e[0] << ',' << v.e[1] << ',' << v.e[2] << ')';
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
    return vec3(u.e[0]+v.e[0], u.e[1]+v.e[1], u.e[2]+v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3 v)
{
    return vec3(u.e[0]-v.e[0], u.e[1]-v.e[1], u.e[2]-v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
    return vec3(u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]);
}

inline vec3 operator*(double t, const vec3& v)
{
    return vec3(v.e[0]*t, v.e[1]*t, v.e[2]*t);
}

inline vec3 operator*(const vec3& v, double t)
{
    return t*v;
}

inline vec3 operator/(const vec3& v, double t)
{
    return v*(1/t);
}

inline double dot(const vec3& u, const vec3& v)
{
    return  u.e[0]*v.e[0] +
            u.e[1]*v.e[1] +
            u.e[2]*v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
    return vec3(
            u.e[1]*v.e[2] - u.e[2]*v.e[1],
            u.e[2]*v.e[0] - u.e[0]*v.e[2],
            u.e[0]*v.e[1] - u.e[1]*v.e[0]
    );
}

inline vec3 unit(const vec3& v)
{
    return v/v.length();
}

inline vec3 random_unit_vector()
{
    while(true)
    {
        vec3 v = vec3::random(-1, 1);
        double v_len_sqr = v.length_squared();

        if(1e-160 < v_len_sqr && v_len_sqr <= 1)
        {
            return v / v_len_sqr;
        }
    }
}

inline vec3 random_in_unit_circle()
{
    while(true)
    {
        vec3 v = vec3(random_double(-1, 1), random_double(-1, 1), 0.);

        if(v.length_squared() < 1)
        {
            return v;
        }
    }
}

inline vec3 random_on_hemisphere(const vec3& normal)
{
    vec3 on_unit_sphere = random_unit_vector();

    if(dot(normal, on_unit_sphere) > .0)
    {
        return on_unit_sphere;
    }

    return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2*(dot(v, n))*n;
}

// See "./maths/refraction.pdf"
inline vec3 refract(const vec3& v, const vec3& n, double n1_over_n2)
{
    vec3 v_unit = unit(v);

    // is negative since v is incident to the surface normal
    double dot_vUnit_n = dot(v_unit, n); 

    // sin(theta) = x / h, with h = 1
    double theta = acos(dot_vUnit_n);

    // min and max to keep the cosine between -1 and 1, else causes asin() error
    double sin_theta_prime = std::min(std::max(n1_over_n2*sin(theta), -1.), 1.);
    double cos_theta_prime = cos(asin(sin_theta_prime));

    // u = sin*xVector - cos*nVector
    vec3 u = sin_theta_prime*unit(v_unit - (n*dot_vUnit_n)) - cos_theta_prime*n;

    return u;
}

#endif