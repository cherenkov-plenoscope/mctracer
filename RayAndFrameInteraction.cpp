#include "Ray.h"
#include "OctTreeTraversingRay.h"
//------------------------------------------------------------------------------
void Ray::find_intersection_candidates_in_tree_of_frames(
	const CartesianFrame* frame, 
	std::vector<const CartesianFrame*> *frames_with_intersection_in_bounding_sphere
)const{
	if(has_intersection_with_bounding_sphere_of(frame)) {

		if(frame->has_children()) {

			if(frame->uses_oct_trees_to_store_its_children()) {
				find_intersections_for_children_in_oct_trees(
					frame,
					frames_with_intersection_in_bounding_sphere
				);
			}else{
				find_intersections_for_all_children_on(
					frame,
					frames_with_intersection_in_bounding_sphere
				);
			}
		}else{
			frames_with_intersection_in_bounding_sphere->push_back(frame);		
		}
	}
}
//------------------------------------------------------------------------------
void Ray::find_intersections_for_children_in_oct_trees(	
	const CartesianFrame* frame, 
	std::vector<const CartesianFrame*> *frames_with_intersection_in_bounding_sphere
)const {
	const OctTreeCube *OctTreeOfFrame2CheckForIntersectionOfRayAndMaxSphere = 
	frame->get_OctTree();
	
	if(OctTreeOfFrame2CheckForIntersectionOfRayAndMaxSphere != NULL){
		// This Frame uses an OctTree to store its children
		std::unordered_set<CartesianFrame*> IntersectionCandidates;

		// initialize a special Ray with additional information for 
		// the OctTree traversal
		OctTreeTraversingRay SpecialRay(this);

		// transform the special Ray to the Cartesian frame where 
		// the OctTree lives
		homo_transformation_of_ray(
			&SpecialRay,
			frame->
			world2frame()
		);

		// update the special information in the special Ray
		SpecialRay.update();

		// find the intersection candidates
		SpecialRay.IntersectionCandidatesInOctTree(
			OctTreeOfFrame2CheckForIntersectionOfRayAndMaxSphere,
			&IntersectionCandidates
		);

		// add the intersection candidates to the list of frames to check for 
		// an intersection
		for(CartesianFrame* FrameFoundInOctTree : IntersectionCandidates){
			find_intersection_candidates_in_tree_of_frames(FrameFoundInOctTree,
				frames_with_intersection_in_bounding_sphere
			);
		}
	}
}
//------------------------------------------------------------------------------
void Ray::find_intersections_for_all_children_on(
	const CartesianFrame* frame, 
	std::vector<const CartesianFrame*> *frames_with_intersection_in_bounding_sphere
)const {
	for(
		uint child_itterator=0; 
		child_itterator<
		frame->
		get_number_of_children();
		child_itterator++
	){
		find_intersection_candidates_in_tree_of_frames(
			frame->
			get_pointer_to_child(child_itterator),
			frames_with_intersection_in_bounding_sphere
		);
	}
}
//------------------------------------------------------------------------------
void Ray::find_intersections_in_intersection_candidate_frames(
	std::vector<const CartesianFrame*> *objects_which_might_intersect,
	std::vector<Intersection*> *intersections_taking_place,
  	const CartesianFrame* object_propagated_from
)const{

	std::vector<Intersection*> intersections_which_might_take_place;

	for(const CartesianFrame* object : *objects_which_might_intersect) {

		// transform ray into object system using homogenous transformation
		Ray ray_in_object_system(support,direction);
		
		homo_transformation_of_ray(
			&ray_in_object_system,
			object->world2frame()
		);
		
		// requesting hit in corresponding object
		// a virtual hit in CartesianFrame decleration is needed to do this
		Intersection* intersection_which_might_take_place;
		intersection_which_might_take_place = new Intersection;
		
		intersections_which_might_take_place.
		push_back(intersection_which_might_take_place);
		
		object->hit(	
			&ray_in_object_system.support,	
			&ray_in_object_system.direction,
			intersections_which_might_take_place.back()
		);
		
		// add object to list of hits when hit_flag == true

		if(intersections_which_might_take_place.back()->get_intersection_flag()
		) {
			if(object != object_propagated_from) {
				intersections_taking_place->push_back(
					intersections_which_might_take_place.back()
				);
			}else{
				intersections_which_might_take_place.
					back()->set_intersection_flag(false);
			}
		}
	}
	// free mempory
	for(Intersection *intersection_which_might_take_place : intersections_which_might_take_place
	) {
		if(!intersection_which_might_take_place->get_intersection_flag()) {
			delete intersection_which_might_take_place;
		}
	}

}
//------------------------------------------------------------------------------
void Ray::calculate_reflected_ray(	
	const Intersection * closest_intersection,
	Ray *ray_reflection_on_object
)const{
	// transform ray_in_object_system into the system of this 
	// particular frame.
	Ray ray_in_object_system;
	ray_in_object_system.SetRay(support,direction);

	homo_transformation_of_ray(
		&ray_in_object_system,
		closest_intersection->
			get_pointer_to_intersecting_object()->world2frame()
	);
	
	Vector3D refl_dir = ray_in_object_system.direction;
	// mirror
	closest_intersection->
		get_reflection_direction_in_object_system(&refl_dir);

	Vector3D refl_sup;
	closest_intersection->
		get_intersection_vec_in_object_system(&refl_sup);
	
	ray_reflection_on_object->SetRay(refl_sup,refl_dir);

	// calculate reflection ray in world system
	homo_transformation_of_ray(
		ray_reflection_on_object,
		closest_intersection->
			get_pointer_to_intersecting_object()->frame2world()
	);	
}
//------------------------------------------------------------------------------
Intersection* Ray::calculate_closest_intersection(	
		std::vector<Intersection*> *intersections
)const{
	// In a list of valid intersectios with this ray and some frames we look 
	// for the intersection which is closest to the support vector of the ray.
	// We return the intersection closest to the support of the vector.
	std::vector<Intersection*>::iterator closest_intersection = min_element( 	
		intersections->begin(),
		intersections->end() ,
		*this
	);

	return 	*closest_intersection;
}
//------------------------------------------------------------------------------
Intersection* Ray::get_closest_intersection(
	const CartesianFrame* world,
	const GlobalSettings *settings
)const {
	std::vector<const CartesianFrame*> objects_which_might_intersect;
	find_intersection_candidates_in_tree_of_frames( 
		world , 
		&objects_which_might_intersect
	);
	
	std::vector<Intersection*> intersections;
	
	const CartesianFrame* object_propagated_from = NULL;
	find_intersections_in_intersection_candidate_frames(
		&objects_which_might_intersect,
		&intersections,
		object_propagated_from
	);
		
	if( intersections.size() == 0 ){
		Intersection *false_intersection;
		false_intersection = new Intersection;
		false_intersection->set_intersection_flag( false );
		return false_intersection;
	}else{
		return calculate_closest_intersection(&intersections);
	}
}
//------------------------------------------------------------------------------
std::vector<Intersection*> Ray::get_intersections(
	const CartesianFrame* world,
	const CartesianFrame* object_propagated_from
)const{

	std::vector<const CartesianFrame*> objects_which_might_intersect;
	find_intersection_candidates_in_tree_of_frames( 
		world, 
		&objects_which_might_intersect 
	);
	
	std::vector<Intersection*> intersections;
	
	find_intersections_in_intersection_candidate_frames(
		&objects_which_might_intersect,
		&intersections,
		object_propagated_from
	);

	return intersections;
}
//------------------------------------------------------------------------------
void Ray::propagate(	
	const CartesianFrame* world, 
	ListOfInteractions* history,
	int interaction_count,
	const CartesianFrame* object_propagated_from,
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator* dice
){
	std::cout << "Calling propagate of a Ray instance!" << endl;
}
//------------------------------------------------------------------------------
double Ray::get_distance_to_closest_object(
	const CartesianFrame *world,
	const GlobalSettings *settings
)const{
	Intersection *closest = get_closest_intersection(world, settings);

	if(closest->get_intersection_flag()) {
		return closest->get_intersection_distance();
	}else{
		return 0.0;
	}
}