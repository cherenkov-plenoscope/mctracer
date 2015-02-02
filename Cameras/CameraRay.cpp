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
ColourProperties CameraRay::trace(
	const CartesianFrame* world,
	int refl_count,
	const CartesianFrame* object_propagated_from,
	const GlobalSettings *settings
)const {

	ColourProperties color;
			
	Intersection* intersection = get_first_intersection(world);

	if(intersection->does_intersect()) {
		if(	
			intersection->get_intersecting_object()->get_reflection_flag() &&
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

			color = intersection->
				get_intersecting_object()->get_color();

			// use itterativ call of trace to handle reflections
			ColourProperties color_of_reflected_ray;
			
			color_of_reflected_ray = ray_reflection_on_object.trace(
				world,
				refl_count,
				intersection->get_intersecting_object(),
				settings
			);
			
			color.reflection_mix(
				&color_of_reflected_ray,
				intersection->
					get_intersecting_object()->
						get_reflection_properties()
			);
			
		}else{
			color = intersection->get_intersecting_object()->get_color();
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
    os << camera_ray_to_be_displayed.get_string();
    return os;
}