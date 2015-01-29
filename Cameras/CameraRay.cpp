#include "CameraRay.h"
//------------------------------------------------------------------------------
CameraRay::CameraRay(const Vector3D support, const Vector3D direction){
	SetRay(support, direction);
}
//------------------------------------------------------------------------------
std::string CameraRay::get_string()const{
	std::stringstream out; 
	out << Ray::get_string() << ", colour: " << colour;
	return out.str();
}
//------------------------------------------------------------------------------
void CameraRay::disp()const{
	std::cout << "CameraRay -> " << get_string() << "\n";
}
//------------------------------------------------------------------------------
/*
ColourProperties CameraRay::trace(
	const CartesianFrame* world,
	int refl_count,
	const CartesianFrame* object_propagated_from,
	const GlobalSettings *settings
)const {

	std::vector<Intersection*> intersections = 
		get_intersections(world,object_propagated_from);

	ColourProperties colour_to_return = settings->get_default_colour();
	
	if(intersections.size()==0) {

		colour_to_return = settings->get_default_colour();
	}else{
		// find the closest object in the list of 
		// list_of_intersecting_objects
		const Intersection *closest_intersection =
			calculate_closest_intersection(&intersections);
		
		if(
			closest_intersection->
				get_pointer_to_intersecting_object()->get_reflection_flag()
			&& 
			settings->max_number_of_reflections_is_not_reached_yet(refl_count)
		) {	
			// the object pointer_to_closest_frame is pointing to
			// does reflect, so we increase the reflection counter
			refl_count++;
			
			// calculate the ray reflected by the object
			// pointer_to_closest_frame is pointing to relativ to
			// the world coordinate system
			CameraRay ray_reflection_on_object;
			calculate_reflected_ray(
				closest_intersection,
				&ray_reflection_on_object
			);

			// mix colours
			colour_to_return = closest_intersection->
			get_pointer_to_intersecting_object()->get_colour();

			// use itterativ call of trace to handle reflections
			ColourProperties colour_of_reflected_ray;
		
			colour_of_reflected_ray = ray_reflection_on_object.trace(
				world,
				refl_count,
				closest_intersection->get_pointer_to_intersecting_object(),
				settings
			);

			colour_to_return.reflection_mix(
				&colour_of_reflected_ray,
				closest_intersection->
					get_pointer_to_intersecting_object()->
						get_reflection_properties()
			);
		}else{

			colour_to_return = closest_intersection->
				get_pointer_to_intersecting_object()->get_colour();
		}	
	}

	for(Intersection *ptr_to_intersection : intersections) {	
		delete ptr_to_intersection;
	}

	return colour_to_return;
}*/
//------------------------------------------------------------------------------
ColourProperties CameraRay::trace(
	const CartesianFrame* world,
	int refl_count,
	const CartesianFrame* object_propagated_from,
	const GlobalSettings *settings
)const {

	ColourProperties color;

	Intersection* intersection = get_first_intersection(world);

	if(intersection->does_intersect()) {
		color = intersection->get_intersecting_object()->get_colour();
	}else{
		color = settings->get_default_colour();
	}

	delete intersection;
	return color;
}
//------------------------------------------------------------------------------
//======================================================================
// friends of osstream
//======================================================================
std::ostream& operator<<(std::ostream& os, 
const CameraRay& camera_ray_to_be_displayed){
    os << camera_ray_to_be_displayed.get_string();
    return os;
}
