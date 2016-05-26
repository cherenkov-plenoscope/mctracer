//=================================
// include guard
#ifndef __RAYANDFRAME_H_INCLUDED__
#define __RAYANDFRAME_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Frame.h"
#include "Core/Ray.h"

namespace RayAndFrame {
	// Frame's bounding sphere
	bool ray_support_inside_frames_bounding_sphere(
		const Ray* ray, 
		const Frame *frame
	);
	bool ray_has_intersection_with_bounding_sphere_of(
		const Ray* ray, 
		const Frame* frame
	);

	Ray get_ray_transformed_in_object_system_of_frame(
		const Ray* ray, 
		const Frame* frame
	);
	/*
	// Causal intersection with Frame
    const Intersection* get_first_intersection_of_ray_in_frame(
    	const Ray* ray, 
    	const Frame* frame
    );
    
    void find_intersection_candidates_in_tree_of_frames(
        const Frame* frame, 
        vector<const Frame*> *candidate_frames
    );
    
    vector<const Frame*> get_intersection_candidate_objects(
        const Frame* frame
    );
    
    vector<const Intersection*> get_intersections_in_candidate_objects(
        vector<const Frame*> *candidate_objects
    );
    
    const Intersection* get_closest_intersection_and_delete_the_rest(   
        vector<const Intersection*> *intersections
    );
    
    const Intersection* calculate_closest_intersection(
        vector<const Intersection*> *intersections
    );
    */
}
#endif // __RAYANDFRAME_H_INCLUDED__ 