//=================================
// include guard
#ifndef __RAY_H_INCLUDED__
#define __RAY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
#include "Core/Vec3.h"
#include "Core/HomTra3.h"
using std::string;
using std::stringstream;
using std::vector;

class Ray {
protected:
    
    Vec3 support;
    Vec3 direction;
public:

    Ray(const Vec3 support, const Vec3 direction);
    void set_direction(const Vec3 ndir);
    void set_support(const Vec3 nsup);
    void set_support_and_direction(const Vec3 nsup, const Vec3 ndir);    
    Vec3 get_support()const;
    Vec3 get_direction()const;
    Vec3 get_pos_at(const double scalar)const;
    void transform(const HomTra3 *T);
    string get_print()const;
    double get_parameter_on_ray_for_closest_distance_to_point(
        const Vec3 &point
    )const;
    double get_closest_distance_to_point(const Vec3 &point)const;
    friend std::ostream& operator<<(std::ostream& os, const Ray& ray) {
        os << ray.get_print();
        return os;
    };
protected:

    Ray();
};
#endif // __RAY_H_INCLUDED__ 