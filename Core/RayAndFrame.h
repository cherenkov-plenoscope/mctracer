// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORE_RAYANDFRAME_H_
#define MCTRACER_CORE_RAYANDFRAME_H_

#include <vector>
#include "Core/Frame.h"
#include "Core/Ray.h"
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"

namespace RayAndFrame {
// Frame's bounding sphere

bool ray_support_inside_frames_bounding_sphere(
    const Ray* ray,
    const Frame *frame);

bool ray_has_intersection_with_bounding_sphere_of(
    const Ray* ray,
    const Frame* frame);

Ray get_ray_transformed_in_object_system_of_frame(
    const Ray* ray,
    const Frame* frame);

Intersection first_intersection(
    const Ray* ray,
    const Frame* frame);

struct CausalIntersection {
    const Ray* ray;
    static thread_local std::vector<const Frame*> candidate_objects;
    static thread_local std::vector<Intersection> candidate_intersections;
    Intersection closest_intersection;
    CausalIntersection(const Ray* _ray, const Frame* frame);
 private:
    void find_intersection_candidates_in_tree_of_frames(const Frame* frame);
    void find_intersections_in_candidate_objects();
    void calculate_closest_intersection();
};

}  // namespace RayAndFrame
#endif  // MCTRACER_CORE_RAYANDFRAME_H_
