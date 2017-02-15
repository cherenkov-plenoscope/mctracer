#include "Propagator.h"

namespace Visual {
const double Propagator::wavelength = 533e-9;
//------------------------------------------------------------------------------
Propagator::Propagator(
	CameraRay* _cray,
	const Frame* _scenery,
	const Config* _config): 
	scenery(_scenery),
	config(_config),
	cray(_cray)
{
	propagate();
}
//------------------------------------------------------------------------------
void Propagator::propagate() {
	isec = RayAndFrame::first_intersection(cray, scenery);

	if(isec.does_intersect())
		interact_with_object();
	else
		reach_sky_dome();	
}
//------------------------------------------------------------------------------
void Propagator::interact_with_object() {
	if(isec.get_facing_reflection_propability(wavelength) >= prng.uniform())
		reflect_on_surface_and_propagate_on();
	else
		reach_boundary_layer(); 
}
//------------------------------------------------------------------------------
void Propagator::reflect_on_surface_and_propagate_on() {
	cray->set_support_and_direction(
		isec.get_intersection_vector_in_world_system(),
		isec.get_reflection_direction_in_world_system(cray->get_direction())
	);
	propagate();
}
//------------------------------------------------------------------------------
void Propagator::reach_boundary_layer() {
	if(isec.boundary_layer_is_transparent())
		fresnel_refraction_and_reflection();
	else
		get_absorbed_on_surface();
}
//------------------------------------------------------------------------------
void Propagator::fresnel_refraction_and_reflection() {
	FresnelRefractionAndReflection fresnel(
		isec.object2world()->
			get_transformed_orientation_inverse(cray->get_direction()),
		isec.get_normal_in_faceing_surface_system(),
		isec.get_refractive_index_coming_from(wavelength),
		isec.get_refractive_index_going_to(wavelength)
	);

	if(fresnel.reflection_propability() > prng.uniform())
		reflect_on_surface_and_propagate_on(); 
	else
		pass_the_boundary_layer(fresnel);
}
//------------------------------------------------------------------------------
void Propagator::get_absorbed_on_surface() {
	color = isec.get_facing_color();	
}
//------------------------------------------------------------------------------
void Propagator::pass_the_boundary_layer(
	const FresnelRefractionAndReflection &fresnel 
) {
	propagate_on_after_boundary_layer(fresnel);
}
//------------------------------------------------------------------------------
void Propagator::propagate_on_after_boundary_layer(
	const FresnelRefractionAndReflection &fresnel
) {
	if(	isec.get_object()->has_restrictions_on_frames_to_propagate_to() && 
		!isec.going_to_default_refractive_index()
	)
		scenery = isec.get_object()->get_allowed_frame_to_propagate_to();
	else
		scenery = isec.get_object()->get_root();

	cray->set_support_and_direction(
		isec.get_intersection_vector_in_world_system(),
		isec.object2world()->get_transformed_orientation(
			fresnel.get_refrac_dir_in_object_system())
	);

	propagate();	
}
//------------------------------------------------------------------------------
void Propagator::reach_sky_dome() {
	color = config->sky_dome.get_color_for_direction(cray->get_direction());
}
//------------------------------------------------------------------------------
}//Visual