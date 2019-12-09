// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_RAYANDFRAME_H_
#define MERLICT_RAYANDFRAME_H_

#include <vector>
#include "merlict/Frame.h"
#include "merlict/Ray.h"
#include "merlict/SurfaceEntity.h"
#include "merlict/Intersection.h"

namespace merlict {

bool ray_support_inside_frames_bounding_sphere(
    const Ray* ray,
    const Frame *frame);

bool ray_intersects_frames_bounding_sphere(
    const Ray* ray,
    std::shared_ptr<const Frame> frame);

Ray ray_with_respect_to_frame(
    const Ray* ray,
    std::shared_ptr<const Frame> frame);

Intersection rays_first_intersection_with_frame(
    const Ray* ray,
    std::shared_ptr<const Frame> frame);

struct CausalIntersection {
    const Ray* ray;
    std::vector<std::shared_ptr<const Frame>> candidate_objects;
    std::vector<Intersection> candidate_intersections;
    Intersection closest_intersection;
    CausalIntersection(const Ray* _ray, std::shared_ptr<const Frame> frame);
 private:
    void find_intersection_candidates_in_tree_of_frames(std::shared_ptr<const Frame> frame);
    void find_intersections_in_candidate_objects();
    void calculate_closest_intersection();
};

}  // namespace merlict

#endif  // MERLICT_RAYANDFRAME_H_
