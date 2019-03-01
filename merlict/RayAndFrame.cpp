// Copyright 2014 Sebastian A. Mueller
#include "merlict/RayAndFrame.h"
#include <algorithm>


namespace merlict {
namespace RayAndFrame {

bool ray_support_inside_frames_bounding_sphere(
    const Ray* ray,
    const Frame *frame
) {
    return (frame->get_position_in_world() - ray->support()).
        norm_is_less_equal_than(frame->get_bounding_sphere_radius());
}

bool ray_has_intersection_with_bounding_sphere_of(
    const Ray* ray,
    const Frame *frame
) {
    const double alpha =
        ray->parameter_for_closest_distance_to_point(
            frame->get_position_in_world());

    const Vec3 q = ray->position_at(alpha);
    const Vec3 shortest_connection = frame->get_position_in_world() - q;
    const double dist_square = shortest_connection*shortest_connection;

    if (
        dist_square >
        frame->get_bounding_sphere_radius()*frame->get_bounding_sphere_radius()
    ) {
        //
        // -------------+-----\--------> ray
        //              |     |
        //            __|__    > dist
        //           /  |  \  |
        //          |   +   | /
        //           \_____/
        //              <--->
        //              radius of sphere enclosing all children
        //
        // The ray does not intersect the frame's boundary sphere.
        return false;
    } else {
        if (alpha < 0.0) {
            // The frame support is behind the source of this ray.
            // We test whether the source of this ray is still inside of the
            // frame, i.e. inside the boundary sphere, or completley behind it.
            if (ray_support_inside_frames_bounding_sphere(ray, frame)) {
                // The support of the ray is inside the frame's boundary sphere
                //
                //               _________
                //              /         \                                   //
                //    ________/_____________\________ray_direction_________\  //
                //           /    /          \                             /
                //          |    /   x frame  |
                //          |   /   /         |
                //           \ /   / radius  /
                //            /   /         /
                //           /  \/________/
                //          /
                //       support
                //
                // The ray intersects the boundary sphere when leaving the
                // sphere.
                return true;
            } else {
                // The support of the ray is outside and behind the frame's
                // boundary sphere with respect to its support vector.
                //
                //               _________
                //              /         \                                   //
                //    ________/_____________\________ray_direction_________\  //
                //           /               \          /                  /
                //          |        x frame  |        /
                //          |       /         |       /
                //           \     / radius  /       /
                //            \   /         /       /
                //              \/________/        /
                //                                /
                //                             support
                //
                // The ray does not intersect the boundary sphere at all since
                // it starts behind the frame's boundary sphere.
                return false;
            }
        } else {
            // the frame support is in front of the source of this ray
            // Closest point on ray to center of frame =
            // ray.support + v * ray.direction
            //
            // here v > 0
            //               _________
            //              /         \                                       //
            //    ________/_____________\________ray_direction_________\      //
            //      /    /               \                             /
            //     /    |        x frame  |
            //    /     |       /         |
            // support   \     / radius  /
            //            \   /         /
            //              \/________/
            //
            // The ray does not intersect the frame's boundary sphere.
            return true;
        }
    }
}

Ray get_ray_transformed_in_object_system_of_frame(
    const Ray* ray,
    const Frame* frame
) {
    Ray ray_in_object_system_of_frame = *ray;
    ray_in_object_system_of_frame.transform_inverse(frame->frame2world());
    return ray_in_object_system_of_frame;
}

thread_local std::vector<const Frame*>
    CausalIntersection::candidate_objects;
thread_local std::vector<Intersection>
    CausalIntersection::candidate_intersections;

Intersection first_intersection(const Ray* ray, const Frame* frame) {
    CausalIntersection intersect_calculator(ray, frame);
    return intersect_calculator.closest_intersection;
}

CausalIntersection::CausalIntersection(
    const Ray* _ray,
    const Frame* frame
): ray(_ray) {
    candidate_objects.clear();
    candidate_intersections.clear();
    find_intersection_candidates_in_tree_of_frames(frame);
    find_intersections_in_candidate_objects();
    calculate_closest_intersection();
}

void CausalIntersection::find_intersection_candidates_in_tree_of_frames(
    const Frame* frame
) {
    if (ray_has_intersection_with_bounding_sphere_of(ray, frame)) {
        if (frame->has_children()) {
            for (unsigned int i = 0; i < frame->get_children()->size(); i++)
                find_intersection_candidates_in_tree_of_frames(
                    frame->get_children()->at(i));
        } else {
            candidate_objects.push_back(frame);
        }
    }
}

void CausalIntersection::find_intersections_in_candidate_objects() {
    for (const Frame* object : candidate_objects) {
        Ray ray_in_object_system =
            get_ray_transformed_in_object_system_of_frame(
                ray,
                object);

        object->calculate_intersection_with(
            &ray_in_object_system,
            &candidate_intersections);
    }
}

void CausalIntersection::calculate_closest_intersection() {
    if (candidate_intersections.size() == 0)
        closest_intersection = Intersection(
            &SurfaceEntity::VOID_SURFACE_ENTITY,
            ray->position_at(1e4),
            ray->direction(),
            1e4,
            ray->direction());
    else
        closest_intersection = *min_element(
            candidate_intersections.begin(),
            candidate_intersections.end(),
            Intersection::compare);
}

}  // namespace RayAndFrame
}  // namespace merlict
