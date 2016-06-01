#include "CameraRay.h"
#include "Core/RayAndFrame.h"
#include "Core/Intersection.h"
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
	const TracerSettings *settings
)const {
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
				reflected_ray.trace(world, refl_count, settings);

			color = intersection.get_facing_color();
			color.reflection_mix(
				reflection_color,
				refl_coeff
			);
		}else{

			if(settings->visual.global_illumination.on)
				color = shadow_of_sky_light(world, settings, intersection);
			else
				color = intersection.get_facing_color();
		}
	}else{
		color = settings->visual.sky_dome.get_color_for_direction(direction);
	}

	return color;
}
//------------------------------------------------------------------------------
Color CameraRay::shadow_of_sky_light(
	const Frame* world,
	const TracerSettings *settings,
	const Intersection &intersection
)const {

	const double max_darkening = 0.750;

	Vec3 specular_dir = intersection.get_reflection_direction_in_world_system(
		settings->visual.global_illumination.incoming_direction
	);

	double darkening = specular_dir*direction;

	if(darkening < 0.0) darkening = 0.0;
	if(darkening > max_darkening) darkening = max_darkening;

	Color color;
	color = intersection.get_facing_color();

	if(!is_iluminated_by_sky_light_source(world, settings, intersection))
		darkening = darkening*0.25;

	color.reflection_mix(Color::black, max_darkening - darkening);	
	
	return color;
}
//------------------------------------------------------------------------------
bool CameraRay::is_iluminated_by_sky_light_source(
	const Frame* world,
	const TracerSettings *settings,
	const Intersection &intersection
)const {

	Ray ray_to_source(
		intersection.get_intersection_vector_in_world_system(),
		settings->visual.global_illumination.incoming_direction
	);

	const Intersection intersec_light_source = 
		RayAndFrame::first_intersection(this, world);

	double p = intersection.get_surface_normal_in_world_system()*
		settings->visual.global_illumination.incoming_direction;

	if(surface_normal_is_facing_camera(intersection))
		p = p*-1.0;	
	
	bool does_intersect = intersec_light_source.does_intersect();

	return !(does_intersect || p < 0.0);
}
//------------------------------------------------------------------------------
bool CameraRay::surface_normal_is_facing_camera(
	const Intersection &intersection
)const {
	return intersection.get_surface_normal_in_world_system()*get_direction() > 0.0;
}