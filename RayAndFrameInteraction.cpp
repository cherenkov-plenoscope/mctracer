#include "Ray.h"
//------------------------------------------------------------------------------
Intersection* Ray::get_first_intersection_in(const CartesianFrame* frame)const {

	std::vector<const CartesianFrame*> candidate_objects =
		get_intersection_candidate_objects(frame);

	std::vector<Intersection*> intersections = 
		get_intersections_in_candidate_objects(&candidate_objects);		

	return get_closest_intersection_and_delete_the_rest(&intersections);
}
//------------------------------------------------------------------------------
std::vector<const CartesianFrame*> Ray::get_intersection_candidate_objects(
	const CartesianFrame* frame
)const {

	std::vector<const CartesianFrame*> candidate_objects;
	find_intersection_candidates_in_tree_of_frames(frame, &candidate_objects);

	return candidate_objects;
}
//------------------------------------------------------------------------------
void Ray::find_intersection_candidates_in_tree_of_frames(
	const CartesianFrame* frame, 
	std::vector<const CartesianFrame*> *candidate_frames
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
std::vector<Intersection*> Ray::get_intersections_in_candidate_objects(
	std::vector<const CartesianFrame*> *candidate_objects
)const{

	std::vector<Intersection*> intersections;

	for(const CartesianFrame* object : *candidate_objects) {

		Ray ray_in_object_system = 
			get_ray_transformed_in_object_system_of(object);

		Intersection* candidate_intersection = 
			object->calculate_intersection_with(&ray_in_object_system);
		
		if(candidate_intersection->does_intersect()) {
			if( !ray_in_object_system.
					support_equals_intersection_point(candidate_intersection))
				intersections.push_back(candidate_intersection);
		}else
			delete candidate_intersection;
	}

	return intersections;
}
//------------------------------------------------------------------------------
bool Ray::support_equals_intersection_point(const Intersection* intersec)const {
	return 	intersec->get_intersection_vector_in_object_system() == Support();
}
//------------------------------------------------------------------------------
Intersection* Ray::calculate_closest_intersection(	
		std::vector<Intersection*> *intersections
)const{

	if(intersections->size() == 0) {

		Intersection* void_intersection;
		void_intersection = new Intersection(
			Intersection::void_object,
			PositionOnRay(1e4),
			direction,
			1e4,
			direction
		);
		return void_intersection;
	}else{

		std::vector<Intersection*>::iterator closest_intersection = min_element( 	
			intersections->begin(),
			intersections->end() ,
			*this
		);

		return 	*closest_intersection;
	}
}
//------------------------------------------------------------------------------
Intersection* Ray::get_closest_intersection_and_delete_the_rest(	
	std::vector<Intersection*> *intersections
)const{
	Intersection* closest_intersection = 
		calculate_closest_intersection(intersections);

	for(Intersection* intersection : *intersections) 
		if(intersection != closest_intersection)
			delete intersection;

	return closest_intersection;
}
//------------------------------------------------------------------------------
void Ray::calculate_reflected_ray(	
	const Intersection * intersec,
	Ray *ray_reflection_on_object
)const{

	Ray ray_in_object_system = get_ray_transformed_in_object_system_of(
		intersec->get_intersecting_object()
	);
	
	Vector3D refl_dir = ray_in_object_system.direction;
	// mirror
	intersec->get_reflection_direction_in_object_system(&refl_dir);

	Vector3D refl_sup = intersec->get_intersection_vector_in_object_system();

	ray_reflection_on_object->SetRay(refl_sup,refl_dir);

	// calculate reflection ray in world system
	homo_transformation_of_ray(
		ray_reflection_on_object,
		intersec->get_intersecting_object()->frame2world()
	);	
}