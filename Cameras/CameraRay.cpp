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
Color CameraRay::trace(
	const Frame* world,
	uint refl_count,
	const TracerSettings *settings
)const {

	Color color;
			
	const Intersection* intersection = get_first_intersection_in(world);

	if(intersection->does_intersect()) {

		double refl_coeff = intersection->get_facing_reflection_propability();
		if(refl_coeff > 0.0) {
			refl_count++;

			CameraRay reflected_ray(
				intersection->get_intersection_vector_in_world_system(),
				intersection->get_reflection_direction_in_world_system(Direction())
			);

			Color reflection_color = 
				reflected_ray.trace(world, refl_count, settings);

			color = intersection->get_facing_color();
			color.reflection_mix(
				&reflection_color,
				refl_coeff
			);

		}else{
			color = intersection->get_facing_color();
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