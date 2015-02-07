#include "CameraRay.h"
//------------------------------------------------------------------------------
CameraRay::CameraRay(const Vector3D support, const Vector3D direction){
	SetRay(support, direction);
}
//------------------------------------------------------------------------------
std::string CameraRay::get_print()const{
	std::stringstream out; 
	out << get_ray_print() << ", colour: " << colour;
	return out.str();
}
//------------------------------------------------------------------------------
ColourProperties CameraRay::trace(
	const CartesianFrame* world,
	uint refl_count,
	const GlobalSettings *settings
)const {

	ColourProperties color;
			
	Intersection* intersection = get_first_intersection_in(world);

	if(intersection->does_intersect()) {
		if(	
			intersection->get_intersecting_object()->does_reflect() &&
			settings->max_number_of_reflections_is_not_reached_yet(refl_count)
		) {
			refl_count++;
			
			// calculate the ray reflected by the object
			// pointer_to_closest_frame is pointing to relativ to
			// the world coordinate system
			CameraRay ray_reflection_on_object;
			calculate_reflected_ray(
				intersection,
				&ray_reflection_on_object
			);

			color = *intersection->get_intersecting_object()->get_color();

			// use itterativ call of trace to handle reflections
			ColourProperties color_of_reflected_ray;
			
			color_of_reflected_ray = ray_reflection_on_object.trace(
				world,
				refl_count,
				settings
			);
			
			color.reflection_mix(
				&color_of_reflected_ray,
				intersection->
					get_intersecting_object()->
						get_reflection_properties()
			);
			
		}else{
			color = *intersection->get_intersecting_object()->get_color();
		}
	}else{
		color = settings->get_default_colour();
	}

	delete intersection;
	return color;
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, 
const CameraRay& camera_ray_to_be_displayed){
    os << camera_ray_to_be_displayed.get_print();
    return os;
}