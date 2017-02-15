#include "Tracer.h"

namespace Visual {
const double Tracer::wavelength = 533e-9;
//------------------------------------------------------------------------------
Tracer::Tracer(
	CameraRay* _cray,
	const Frame* _scenery,
	const Config* _config): 
	scenery(_scenery),
	config(_config),
	cray(_cray)
{
	trace_back();
}
//------------------------------------------------------------------------------
void Tracer::trace_back() {
	isec = RayAndFrame::first_intersection(cray, scenery);

	if(isec.does_intersect())
		trace_back_to_object_interaction();
	else
		trace_back_to_sky_dome();	
}
//------------------------------------------------------------------------------
void Tracer::trace_back_to_object_interaction() {
	if(isec.get_facing_reflection_propability(wavelength) >= prng.uniform())
		trace_back_after_reflection();
	else
		trace_back_to_boundary_layer(); 
}
//------------------------------------------------------------------------------
void Tracer::trace_back_after_reflection() {
	cray->set_support_and_direction(
		isec.get_intersection_vector_in_world_system(),
		isec.get_reflection_direction_in_world_system(cray->get_direction())
	);
	trace_back();
}
//------------------------------------------------------------------------------
void Tracer::trace_back_to_boundary_layer() {
	if(isec.boundary_layer_is_transparent())
		trace_back_to_fresnel_interaction();
	else
		trace_back_to_surface();
}
//------------------------------------------------------------------------------
void Tracer::trace_back_to_fresnel_interaction() {
	FresnelRefractionAndReflection fresnel(
		isec.object2world()->
			get_transformed_orientation_inverse(cray->get_direction()),
		isec.get_normal_in_faceing_surface_system(),
		isec.get_refractive_index_coming_from(wavelength),
		isec.get_refractive_index_going_to(wavelength)
	);

	if(fresnel.reflection_propability() > prng.uniform())
		trace_back_after_reflection(); 
	else
		trace_back_beyond_boundary_layer(fresnel);
}
//------------------------------------------------------------------------------
void Tracer::trace_back_to_surface() {
	color = isec.get_facing_color();	
}
//------------------------------------------------------------------------------
void Tracer::trace_back_beyond_boundary_layer(
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

	trace_back();	
}
//------------------------------------------------------------------------------
void Tracer::trace_back_to_sky_dome() {
	color = config->sky_dome.get_color_for_direction(cray->get_direction());
}
//------------------------------------------------------------------------------
}//Visual