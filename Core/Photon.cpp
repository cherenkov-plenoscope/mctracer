#include "Photon.h"
#include "PhysicalConstants.h"
//------------------------------------------------------------------------------
Photon::Photon(
	const Vector3D support,
	const Vector3D direction,
	const double wavelength
) {
	SetRay(support, direction);
	this->wavelength = wavelength;
	assert_wavelength_is_positive();
	init_propagation_history();	
}
//------------------------------------------------------------------------------
Photon::Photon(
	const Vector3D support,
	const Vector3D direction,
	const double wavelength,
	const PhotonMcTruth* mc_truth
) {
	SetRay(support, direction);
	this->wavelength = wavelength;
	this->mc_truth = mc_truth;
	assert_wavelength_is_positive();
	init_propagation_history();	
}
//------------------------------------------------------------------------------
void Photon::assert_wavelength_is_positive()const {
	
	if(wavelength <= 0.0 ) {
		std::stringstream info;
		info << __FILE__ << " " << __LINE__ << "\n";
		info << "Expected photon wavelength lambda > 0.0nm, but actual ";
		info << "it is: " << wavelength*1e9 << "nm\n";
		throw BadWaveLength(info.str());
	}
}
//------------------------------------------------------------------------------
Photon::Photon(
	const Photon* photon_to_be_carried_on
) {
	carry_on_propagation_properties_of_ray(photon_to_be_carried_on);
	wavelength = photon_to_be_carried_on->wavelength;
}
//------------------------------------------------------------------------------
double Photon::get_wavelength()const {
	return wavelength;
}
//------------------------------------------------------------------------------
const PhotonMcTruth* Photon::get_mc_truth()const{
	return mc_truth;
}
//------------------------------------------------------------------------------
std::string Photon::get_print()const {
	std::stringstream out; 
	out << RayForPropagation::get_print();
	out << "wavelength " << wavelength*1e9 << "nm\n";
	return out.str();
}
//------------------------------------------------------------------------------
void Photon::propagate_in(PropagationEnvironment environment) {
	this->environment = environment;

	if(limit_of_interactions_is_not_reached_yet()) 
		work_on_first_causal_intersection();
}
//------------------------------------------------------------------------------
void Photon::work_on_first_causal_intersection() {
	
	intersection = get_first_intersection_in(environment.world_geometry);

	if(intersection->does_intersect())
		interact_with_object();
	else
		get_absorbed_in_void_space();
}
//------------------------------------------------------------------------------
void Photon::interact_with_object() {

	if(	intersection->get_facing_reflection_propability(wavelength) >= 
		environment.random_engine->uniform() 
	) {
		reflect_on_surface_and_propagate_on(reflection_on_surface);
	}else{
		reach_boundary_layer(); 
	}
}
//------------------------------------------------------------------------------
void Photon::reflect_on_surface_and_propagate_on(const InteractionType type) {

	Photon reflected_photon(this);

	reflected_photon.SetRay(
		intersection->get_intersection_vector_in_world_system(),
		intersection->get_reflection_direction_in_world_system(Direction())
	);

	push_back_intersection_and_type_to_propagation_history(
		intersection, 
		type
	);

	reflected_photon.propagate_in(environment);
}
//------------------------------------------------------------------------------
void Photon::reach_boundary_layer() {

	if(intersection->boundary_layer_is_transparent())
		fresnel_refraction_and_reflection();
	else
		get_absorbed_on_surface();
}
//------------------------------------------------------------------------------
void Photon::fresnel_refraction_and_reflection() {

	FresnelRefractionAndReflection fresnel(
		intersection->world2object()->get_transformed_orientation(Direction()),
		intersection->get_normal_in_faceing_surface_system(),
		intersection->get_refractive_index_coming_from(wavelength),
		intersection->get_refractive_index_going_to(wavelength)
	);

	if(fresnel.reflection_propability() > environment.random_engine->uniform())
		reflect_on_surface_and_propagate_on(fresnel_reflection_on_surface); 
	else
		pass_the_boundary_layer(fresnel);
}
//------------------------------------------------------------------------------
void Photon::pass_the_boundary_layer(
	const FresnelRefractionAndReflection &fresnel 
) {
	
	if(intersection->from_outside_to_inside())
		push_back_intersection_and_type_to_propagation_history(
			intersection, 
			refraction_to_inside
		);
	else
		push_back_intersection_and_type_to_propagation_history(
			intersection, 
			refraction_to_outside
		);			

	propagate_on_after_boundary_layer(fresnel);
} 
//------------------------------------------------------------------------------
void Photon::propagate_on_after_boundary_layer(
	const FresnelRefractionAndReflection &fresnel
) {

	PropagationEnvironment next_environment = environment;
	
	if(	intersection->get_object()->has_restrictions_on_frames_to_propagate_to() && 
		!intersection->going_to_default_refractive_index()
	)
		next_environment.world_geometry = 
			intersection->get_object()->get_allowed_frame_to_propagate_to();
	else
		next_environment.world_geometry = 
			intersection->get_object()->get_root_of_world();

	Photon refracted_photon(this);

	refracted_photon.SetRay(
		intersection->get_intersection_vector_in_world_system(),
		intersection->object2world()->get_transformed_orientation(
			fresnel.get_refrac_dir_in_object_system()
		)
	);

	refracted_photon.propagate_in(next_environment);	
}
//------------------------------------------------------------------------------
void Photon::get_absorbed_on_surface() {
	push_back_intersection_and_type_to_propagation_history(
		intersection, 
		absorption_on_surface
	);	
}
//------------------------------------------------------------------------------
void Photon::get_absorbed_in_void_space() {
	push_back_intersection_and_type_to_propagation_history(
		intersection, 
		absorption_in_void
	);	
}
//------------------------------------------------------------------------------
bool Photon::limit_of_interactions_is_not_reached_yet()const {
	return environment.propagation_options->
		max_number_of_reflections_is_not_reached_yet(
			get_number_of_interactions_so_far());
}
//------------------------------------------------------------------------------
double Photon::get_time_to_pass_distance_in_refractive_index(
	const double distance_in_medium,
	const double refractive_index_in_medium
)const {
	return refractive_index_in_medium * 
		distance_in_medium / PhysicalConstants::speed_of_light_in_vacuum;
}
//------------------------------------------------------------------------------
double Photon::get_time_of_flight()const {

	double time_of_flight = 0.0;

	uint i =0;
	for(const Intersection* intersec : *intersection_history){

		if(i!=0)
			time_of_flight += get_time_to_pass_distance_in_refractive_index(
				intersec->get_intersection_distance(),
				intersec->get_refractive_index_coming_from(wavelength)
			);

		i++;
	}
	
	return time_of_flight;
}
//------------------------------------------------------------------------------
void Photon::delete_history() {
	
	if( mc_truth != &PhotonMcTruth::void_truth ){
		delete mc_truth;
	}

	for(uint i=0; i<intersection_history->size(); i++)
		delete intersection_history->at(i);
	delete intersection_history;
	
	interaction_type_history->clear();
	delete interaction_type_history;
}
//------------------------------------------------------------------------------