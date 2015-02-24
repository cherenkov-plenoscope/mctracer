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

		InteractionSurfaceFinder surface_finder(this, intersection);

		if(surface_finder.has_surface_interaction()) {
			if(surface_finder.get_interaction_surface()->has_color())
				color = *surface_finder.get_interaction_surface()->get_color();

			if(surface_finder.get_interaction_surface()->has_reflection()) {
				if(surface_finder.get_interaction_surface()->get_reflection()->flag()) {
					// reflection takes place

					refl_count++;

					CameraRay reflected_ray(
						intersection->get_intersection_vector_in_world_system(),
						intersection->get_reflection_direction_in_world_system(Direction())
					);

					ColourProperties reflection_color = 
						reflected_ray.trace(world, refl_count, settings);

					color = *surface_finder.get_interaction_surface()->get_color();
					color.reflection_mix(
						&reflection_color,
						surface_finder.get_interaction_surface()->get_reflection()
					);
				}
			}
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