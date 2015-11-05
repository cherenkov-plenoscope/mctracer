#include "Core/Ray.h"
//------------------------------------------------------------------------------
const Intersection* Ray::get_first_intersection_in(const Frame* frame)const {

	std::vector<const Frame*> candidate_objects =
		get_intersection_candidate_objects(frame);

	std::vector<const Intersection*> intersections = 
		get_intersections_in_candidate_objects(&candidate_objects);		

	return get_closest_intersection_and_delete_the_rest(&intersections);
}
//------------------------------------------------------------------------------
std::vector<const Frame*> Ray::get_intersection_candidate_objects(
	const Frame* frame
)const {

	std::vector<const Frame*> candidate_objects;
	find_intersection_candidates_in_tree_of_frames(frame, &candidate_objects);

	return candidate_objects;
}
//------------------------------------------------------------------------------
void Ray::find_intersection_candidates_in_tree_of_frames(
	const Frame* frame, 
	std::vector<const Frame*> *candidate_frames
)const{
	
	if(has_intersection_with_bounding_sphere_of(frame)) {

		if(frame->has_children()) {
			frame->find_intersection_candidates_for_all_children_and_ray(
				this, candidate_frames
			);
		}else{
			candidate_frames->push_back(frame);		
		}
	}
}
//------------------------------------------------------------------------------
std::vector<const Intersection*> Ray::get_intersections_in_candidate_objects(
	std::vector<const Frame*> *candidate_objects
)const{

	std::vector<const Intersection*> intersections;

	for(const Frame* object : *candidate_objects) {

		Ray ray_in_object_system = 
			get_ray_transformed_in_object_system_of(object);

		const Intersection* candidate_intersection = 
			object->calculate_intersection_with(&ray_in_object_system);
		
		// to prevent a reflected ray to get stucked on the reflective surface
		// again, the candidate intersection is neglected when the reflected 
		// rays support equals the candidate intersectionpoint.
		if(candidate_intersection->does_intersect()) {
			if( !ray_in_object_system.
					support_equals_intersection_point(candidate_intersection))
				intersections.push_back(candidate_intersection);
		}else{
			delete candidate_intersection;
		}
	}

	return intersections;
}
//------------------------------------------------------------------------------
bool Ray::support_equals_intersection_point(const Intersection* intersec)const {
	return 	intersec->get_intersection_vector_in_object_system() == Support();
}
//------------------------------------------------------------------------------
const Intersection* Ray::calculate_closest_intersection(	
		std::vector<const Intersection*> *intersections
)const{

	if(intersections->size() == 0) {

		Intersection* void_intersection;
		void_intersection = new Intersection(
			SurfaceEntity::void_object,
			PositionOnRay(1e4),
			direction,
			1e4,
			direction
		);
		return void_intersection;
	}else{

		std::vector<const Intersection*>::iterator closest_intersection = min_element( 	
			intersections->begin(),
			intersections->end() ,
			Intersection::compare
		);

		return 	*closest_intersection;
	}
}
//------------------------------------------------------------------------------
const Intersection* Ray::get_closest_intersection_and_delete_the_rest(	
	std::vector<const Intersection*> *intersections
)const{
	const Intersection* closest_intersection = 
		calculate_closest_intersection(intersections);

	for(const Intersection* intersection : *intersections) 
		if(intersection != closest_intersection)
			delete intersection;

	return closest_intersection;
}