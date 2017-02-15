#include "CameraRay.h"
#include "Core/RayAndFrame.h"
#include "Core/Intersection.h"
#include "Tracer.h"

namespace Visual {
//------------------------------------------------------------------------------
CameraRay::CameraRay(const Vec3 support, const Vec3 direction){
	set_support_and_direction(support, direction);
}
//------------------------------------------------------------------------------
std::string CameraRay::get_print()const{
	std::stringstream out; 
	out << Ray::get_print() << ", color: " << color;
	return out.str();
}
//------------------------------------------------------------------------------
Color CameraRay::trace(
	const Frame* world,
	uint refl_count,
	const Config *visual_config
) {

	Tracer tracer(
		this,
		world,
		visual_config);
	
	return tracer.color;

	/*
	Color color;
	const Intersection intersection = 
		RayAndFrame::first_intersection(this, world);

	if(intersection.does_intersect()) {

		double refl_coeff = intersection.get_facing_reflection_propability(533e-9);
		if(refl_coeff > 0.0) {
			refl_count++;

			CameraRay reflected_ray(
				intersection.get_intersection_vector_in_world_system(),
				intersection.get_reflection_direction_in_world_system(get_direction())
			);

			Color reflection_color = 
				reflected_ray.trace(world, refl_count, visual_config);

			color = intersection.get_facing_color();
			color.reflection_mix(
				reflection_color,
				refl_coeff
			);
		}else{

			if(visual_config->global_illumination.on)
				color = shadow_of_sky_light(world, visual_config, intersection);
			else
				color = intersection.get_facing_color();
		}
	}else{
		color = visual_config->sky_dome.get_color_for_direction(direction);
	}
	
	return color;
	*/
}
//------------------------------------------------------------------------------
}//Visual