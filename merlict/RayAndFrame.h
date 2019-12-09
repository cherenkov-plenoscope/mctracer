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
    const std::shared_ptr<Frame> frame);

Ray ray_with_respect_to_frame(
    const Ray* ray,
    const std::shared_ptr<Frame> frame);

Intersection rays_first_intersection_with_frame(
    const Ray* ray,
    const std::shared_ptr<Frame> frame);

struct CausalIntersection {
    const Ray* ray;
    std::vector<const std::shared_ptr<Frame>> candidate_objects;
    std::vector<Intersection> candidate_intersections;
    Intersection closest_intersection;
    CausalIntersection(const Ray* _ray, const std::shared_ptr<Frame> frame);
 private:
    void find_intersection_candidates_in_tree_of_frames(const std::shared_ptr<Frame> frame);
    void find_intersections_in_candidate_objects();
    void calculate_closest_intersection();
};

}  // namespace merlict

#endif  // MERLICT_RAYANDFRAME_H_
