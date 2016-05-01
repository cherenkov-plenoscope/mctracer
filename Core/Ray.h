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
#include "Core/Frame.h"
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
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
    Ray get_ray_transformed_in_object_system_of(const Frame* frame)const;
    //--------------------------------------------------------------------------
    // Ray and bounding sphere of Frame

    bool support_of_ray_is_inside_bounding_sphere_of(const Frame *frame)const;
    bool has_intersection_with_bounding_sphere_of(const Frame* frame)const;
    //--------------------------------------------------------------------------
    // Ray and Frame
    
    const Intersection* get_first_intersection_in(const Frame* frame)const;
    void find_intersection_candidates_in_tree_of_frames(
        const Frame* frame, 
        vector<const Frame*> *candidate_frames
    )const;
    friend std::ostream& operator<<(std::ostream& os, const Ray& to_be_displayed) {
        os << to_be_displayed.get_print();
        return os;
    };
    vector<const Frame*> get_intersection_candidate_objects(
        const Frame* frame
    )const;
    vector<const Intersection*> get_intersections_in_candidate_objects(
        vector<const Frame*> *candidate_objects
    )const;
    const Intersection* get_closest_intersection_and_delete_the_rest(   
        vector<const Intersection*> *intersections
    )const;
    const Intersection* calculate_closest_intersection(
        vector<const Intersection*> *intersections
    )const;
    bool support_equals_intersection_point(const Intersection* intersec)const;
protected:

    Ray();
};
#endif // __RAY_H_INCLUDED__ 