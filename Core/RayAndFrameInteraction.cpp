#include "Core/Ray.h"
#include "Core/RayAndFrame.h"
//------------------------------------------------------------------------------
const Intersection* Ray::get_first_intersection_in(const Frame* frame)const {

	vector<const Frame*> candidate_objects =
		get_intersection_candidate_objects(frame);

	vector<const Intersection*> intersections = 
		get_intersections_in_candidate_objects(&candidate_objects);		

	return get_closest_intersection_and_delete_the_rest(&intersections);
}
//------------------------------------------------------------------------------
vector<const Frame*> Ray::get_intersection_candidate_objects(
	const Frame* frame
)const {

	vector<const Frame*> candidate_objects;
	find_intersection_candidates_in_tree_of_frames(frame, &candidate_objects);

	return candidate_objects;
}
//------------------------------------------------------------------------------
void Ray::find_intersection_candidates_in_tree_of_frames(
	const Frame* frame, 
	vector<const Frame*> *candidate_frames
)const{
	
	if(RayAndFrame::ray_has_intersection_with_bounding_sphere_of(this ,frame)) {

		if(frame->has_children()) {

			for(uint i=0; i<frame->get_children()->size(); i++) {
				find_intersection_candidates_in_tree_of_frames(
					frame->get_children()->at(i), 
					candidate_frames
				);		
			}
		}else{
			candidate_frames->push_back(frame);		
		}
	}
}
//------------------------------------------------------------------------------
vector<const Intersection*> Ray::get_intersections_in_candidate_objects(
	vector<const Frame*> *candidate_objects
)const{

	vector<const Intersection*> intersections;

	for(const Frame* object : *candidate_objects) {

		Ray ray_in_object_system = 
			RayAndFrame::get_ray_transformed_in_object_system_of_frame(
				this,
				object
			);

		object->calculate_intersection_with(
			&ray_in_object_system, 
			&intersections
		);
	}

	return intersections;
}
//------------------------------------------------------------------------------
const Intersection* Ray::calculate_closest_intersection(	
		vector<const Intersection*> *intersections
)const{

	if(intersections->size() == 0) {

		Intersection* void_intersection;
		void_intersection = new Intersection(
			SurfaceEntity::void_object,
			get_pos_at(1e4),
			direction,
			1e4,
			direction
		);
		return void_intersection;
	}else{

		vector<const Intersection*>::iterator closest_intersection = min_element( 	
			intersections->begin(),
			intersections->end() ,
			Intersection::compare
		);

		return 	*closest_intersection;
	}
}
//------------------------------------------------------------------------------
const Intersection* Ray::get_closest_intersection_and_delete_the_rest(	
	vector<const Intersection*> *intersections
)const{
	const Intersection* closest_intersection = 
		calculate_closest_intersection(intersections);

	for(const Intersection* intersection : *intersections) 
		if(intersection != closest_intersection)
			delete intersection;

	return closest_intersection;
}