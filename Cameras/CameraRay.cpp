#include "CameraRay.h"
//------------------------------------------------------------------------------
CameraRay::CameraRay(const Vector3D support, const Vector3D direction){
	SetRay(support, direction);
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
			
	const Intersection* intersection = get_first_intersection_in(world);

	if(intersection->does_intersect()) {

		double refl_coeff = intersection->get_facing_reflection_propability(533e-9);
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
				reflection_color,
				refl_coeff
			);
		}else{

			if(settings->sky_light_source)
				color = shadow_of_sky_light(world, settings, intersection);
			else
				color = intersection->get_facing_color();
		}
	}else{
		color = settings->get_sky_dome()->get_color_for_direction(direction);
	}

	delete intersection;
	return color;
}
//------------------------------------------------------------------------------
/*Color CameraRay::shadow_of_sky_light(
	const Frame* world,
	const TracerSettings *settings,
	const Intersection *intersection
)const {

	const double max_darkening = 0.750;
	const double diffuse = 0.1;

	if(is_iluminated_by_sky_light_source(world, settings, intersection)) {

		Vector3D specular_dir = intersection->
		get_reflection_direction_in_world_system(
			settings->global_light_direction
		);

		double specular = acos(specular_dir*direction*-1.0); //0 to Pi
		specular =  specular/M_PI; //0 to 1

		Color color = intersection->get_facing_color();

		if(specular > 0.5) {


			color.reflection_mix(Color::white, specular);	
		}else{
			color.reflection_mix(Color::black, 1.0 -specular);	
		}

		return color;
	}else{

		Color color;
		color = intersection->get_facing_color();
		color.reflection_mix(Color::black, max_darkening + diffuse);	
		return color;		
	}
}*/
//------------------------------------------------------------------------------
Color CameraRay::shadow_of_sky_light(
	const Frame* world,
	const TracerSettings *settings,
	const Intersection *intersection
)const {

	const double max_darkening = 0.750;

	Vector3D specular_dir = intersection->
	get_reflection_direction_in_world_system(
		settings->global_light_direction
	);

	double darkening = specular_dir*direction;

	if(darkening < 0.0) darkening = 0.0;
	if(darkening > max_darkening) darkening = max_darkening;

	Color color;
	color = intersection->get_facing_color();

	if(!is_iluminated_by_sky_light_source(world, settings, intersection))
		darkening = darkening*0.25;

	color.reflection_mix(Color::black, max_darkening - darkening);	
	
	return color;
}
//------------------------------------------------------------------------------
bool CameraRay::is_iluminated_by_sky_light_source(
	const Frame* world,
	const TracerSettings *settings,
	const Intersection *intersection
)const {

	Ray ray_to_source(
		intersection->get_intersection_vector_in_world_system(),
		settings->global_light_direction
	);

	const Intersection* intersec_light_source = 
		ray_to_source.get_first_intersection_in(world);

	double p = intersection->get_surface_normal_in_world_system()*
		settings->global_light_direction;

	if(surface_normal_is_facing_camera(intersection))
		p = p*-1.0;	
	
	bool does_intersect = intersec_light_source->does_intersect();
	delete intersec_light_source;

	return !(does_intersect || p < 0.0);
}
//------------------------------------------------------------------------------
bool CameraRay::surface_normal_is_facing_camera(
	const Intersection *intersection
)const {
	return intersection->get_surface_normal_in_world_system()*Direction() > 0.0;
}