#include "Core/RayAndFrameFirstIntersection.h"
//#include <thread>
namespace RayAndFrame {
//------------------------------------------------------------------------------
SmartIntersection::SmartIntersection(const Ray* _ray, const Frame* _frame):
    ray(_ray), 
    frame(_frame)
{


}
//------------------------------------------------------------------------------
void SmartIntersection::walk_through_intersections_with(const Frame* frame) {
    
    if(ray_has_intersection_with_bounding_sphere_of(ray, frame)) {
        
        if(frame->has_children()) {
            
            vector<FrameDist> sorted_children = sorted_children_of(frame);
            for(const FrameDist &frame_dist: sorted_children)
                walk_through_intersections_with(frame_dist->frame);
        }else{
            check_intersection_with(frame);
        }
    }
}
//------------------------------------------------------------------------------
void SmartIntersection::check_intersection_with(const Frame* object) {
    Ray ray_in_object_system = get_ray_transformed_in_object_system_of_frame(
        ray,
        object
    );

    frame->calculate_intersection_with(
        &ray_in_object_system, 
        &candidate_intersections
    );
}
//------------------------------------------------------------------------------
vector<FrameDist> SmartIntersection::sorted_children_of(const Frame* frame)const {
    
    vector<FrameDist> sorted_children;
    for(const Frame* child: frame->get_children())
        if(ray_has_intersection_with_bounding_sphere_of(ray, child))
            sorted_children.push_back(FrameDist(ray, frame));

    std::sort(sorted_children.begin(), sorted_children.end());

    return sorted_children;
}
//------------------------------------------------------------------------------
FrameDist::FrameDist(const Ray* ray, const Frame* _frame):
    frame(_frame), 
    projected_distance(
        ray->get_parameter_on_ray_for_closest_distance_to_point(
            frame->get_position_in_world()
        )
    )
{}
//------------------------------------------------------------------------------
double FrameDist::get_bounding_sphere_radius()const {
    return frame->get_bounding_sphere_radius();
}
//------------------------------------------------------------------------------
bool FrameDist::operator()(const FrameDist &a, const FrameDist &b) {
    return a.projected_distance < b.projected_distance;
}
//------------------------------------------------------------------------------
} //RayAndFrame