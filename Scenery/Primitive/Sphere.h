//=================================
// include guard
#ifndef __SPHERE_H_INCLUDED__
#define __SPHERE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "Scenery/Geometry/QuadraticEquation.h"

//=================================
class Sphere :public SurfaceEntity{

protected:
    
    double radius;
public:

    Sphere();
    Sphere(const string name, const Vec3 pos, const Rot3 rot);
    void set_radius(double nradius);
    string str()const;
    void calculate_intersection_with(
        const Ray* ray, 
        vector<Intersection> *intersections
    )const;
private:

    bool facing_sphere_from_outside_given_p_m(
        const double v_Plus,
        const double v_Minus
    )const;

    bool facing_away_from_outside_given_p_m(
        const double v_Plus,
        const double v_Minus
    )const;

    void add_sphere_intersection_for_ray_parameter(
        const Ray* ray, 
        const double ray_parameter,
        vector<Intersection> *intersections
    )const;

    QuadraticEquation get_ray_parameter_equation_for_intersections_with_sphere(
        const Ray* ray
    )const;
public:

    class BadRadius :public TracerException{
        using TracerException::TracerException;
    };
};
#endif // __SPHERE_H_INCLUDED__
