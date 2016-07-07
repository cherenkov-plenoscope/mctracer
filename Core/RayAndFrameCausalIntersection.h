//=================================
// include guard
#ifndef __RAYANDFRAME_CAUSAL_INTERSECTIO_H_INCLUDED__
#define __RAYANDFRAME_CAUSAL_INTERSECTIO_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/RayAndFrame.h"

namespace RayAndFrame {
    
    struct FrameDist;

    struct SmartIntersection {

        const Ray* ray;
        const Frame* frame;
        Intersection closest_intersection;

        static thread_local vector<FrameDist> sorted_children;

        SmartIntersection(const Ray* ray, const Frame* frame);
        void set_up_sorted_children_of(const Frame* frame)const;
    };

    struct FrameDist {

        const Frame* frame;
        double projected_distance;

        FrameDist(const Ray* ray, const Frame* _frame);
        double get_bounding_sphere_radius()const;
        bool operator()(const FrameDist &a, const FrameDist &b);
    };
}
#endif // __RAYANDFRAME_CAUSAL_INTERSECTIO_H_INCLUDED__ 