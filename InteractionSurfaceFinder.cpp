#include "InteractionSurfaceFinder.h"
//------------------------------------------------------------------------------
InteractionSurfaceFinder::InteractionSurfaceFinder(
	const Ray* ray, const Intersection* intersec
){
	if(ray_is_in_hemisphere_with_surface_normal(ray, intersec)){
		is_outer_surface = true;
		if(intersec->get_intersecting_object()->has_outer_surface())
			surf = intersec->get_intersecting_object()->get_outer_surface();
	}else{
		is_outer_surface = false;
		if(intersec->get_intersecting_object()->has_inner_surface())
			surf = intersec->get_intersecting_object()->get_inner_surface();
	}
}
//------------------------------------------------------------------------------
bool InteractionSurfaceFinder::has_surface_interaction()const{
	return surf != nullptr;
}
//------------------------------------------------------------------------------
bool InteractionSurfaceFinder::has_surface_interaction_on_outer_surface()const {
	return is_outer_surface;
}
//------------------------------------------------------------------------------
const SurfaceProperties* InteractionSurfaceFinder::get_interaction_surface()const{
	return surf;
}
//------------------------------------------------------------------------------
bool InteractionSurfaceFinder::ray_is_in_hemisphere_with_surface_normal(
	const Ray* ray, const Intersection* intersec
)const{
	return intersec->
		get_surface_normal_in_world_system()*ray->Direction() < 0.0;
}