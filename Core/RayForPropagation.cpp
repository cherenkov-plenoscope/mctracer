#include "RayForPropagation.h"
//------------------------------------------------------------------------------
RayForPropagation::RayForPropagation() {
}
//------------------------------------------------------------------------------
RayForPropagation::RayForPropagation(
	const Vec3 support,
	const Vec3 direction
) {
	set_support_and_direction(support, direction);
	init_propagation_history();
}
//------------------------------------------------------------------------------
void RayForPropagation::init_propagation_history() {
	intersection_history = new vector<Intersection>;	
	interaction_type_history = new vector<InteractionType>;

	push_back_production_of_ray();
}
//------------------------------------------------------------------------------
void RayForPropagation::push_back_production_of_ray() {

	Intersection production_intersection = Intersection(
		SurfaceEntity::source_object,
		support,
		direction, // set normal of production obj to direction of ray
		0.0,
		direction
	);

	push_back_intersection_and_type_to_propagation_history(
		production_intersection,
		production
	);	
}
//------------------------------------------------------------------------------
RayForPropagation::RayForPropagation(
	const RayForPropagation* ray_to_be_carried_on
) {
	carry_on_propagation_properties_of_ray(ray_to_be_carried_on);
}
//------------------------------------------------------------------------------
void RayForPropagation::carry_on_propagation_properties_of_ray(
	const RayForPropagation* ray_to_be_carried_on
) {	
	identifier_number = ray_to_be_carried_on->identifier_number;
	intersection_history = ray_to_be_carried_on->intersection_history;
	interaction_type_history = ray_to_be_carried_on->interaction_type_history;
}
//------------------------------------------------------------------------------
void RayForPropagation::delete_history() {

	intersection_history->clear();
	delete intersection_history;
	
	interaction_type_history->clear();
	delete interaction_type_history;
}
//------------------------------------------------------------------------------
void RayForPropagation::set_id(const uint identifier_number) {
	this->identifier_number = identifier_number;
}
//------------------------------------------------------------------------------
uint RayForPropagation::get_id()const {
	return identifier_number;
}
//------------------------------------------------------------------------------
string RayForPropagation::get_print()const {
	stringstream out;
	out << Ray::get_print() << ", ";
	out << "ID: " << identifier_number << ", Interactions: " ;
	out << get_number_of_interactions_so_far() << "\n";
	out << get_history_print();
	return out.str();
}
//------------------------------------------------------------------------------
string RayForPropagation::get_history_print()const {
	stringstream out;
	int index = 0;
	for(InteractionType type : *interaction_type_history) {
		out << ++index << ") " << get_type_print(type) << " in ";
		out << intersection_history->at(index-1).get_object()->get_name();
		out << " " << intersection_history->at(index-1).	
			get_intersection_vector_in_world_system() << ", dist to prev.:";

		if(index>1) {
			out << intersection_history->at(index-1).get_intersection_vector_in_world_system().distance_to(
			   		intersection_history->at(index-2).get_intersection_vector_in_world_system()
				)*1e9 << "nm";
		}

		out << "\n";
	}
	return out.str();
}
//------------------------------------------------------------------------------
void RayForPropagation::push_back_intersection_and_type_to_propagation_history(
	const Intersection &interact, 
	const InteractionType type
) {
	intersection_history->push_back(interact);
	interaction_type_history->push_back(type);	
}
//------------------------------------------------------------------------------
double RayForPropagation::get_accumulative_distance()const {
  	
  	double accumulative_distance = 0.0;
	
	for(const Intersection &intersection : *intersection_history) 
      	accumulative_distance += intersection.get_intersection_distance();

    return accumulative_distance;	
}
//------------------------------------------------------------------------------
void RayForPropagation::propagate_in(PropagationEnvironment env) {
	throw TracerException("Prototype RayForPropagation must not be propagated itself, only its children such as the photon are meant to be propagated.");
}
//------------------------------------------------------------------------------
uint RayForPropagation::get_number_of_interactions_so_far()const {
	return uint(intersection_history->size());
}
//------------------------------------------------------------------------------
const Intersection& RayForPropagation::get_intersection_at(
	const uint index
)const{
	return intersection_history->at(index);
}
//------------------------------------------------------------------------------
string RayForPropagation::get_type_print(const InteractionType type)const {
	switch(type) {
		case production: return "production"; break;
		case absorption_in_void: return "absorption_in_void"; break;

		case absorption_in_medium: return "absorption_in_medium"; break;
		case absorption_on_surface: return "absorption_on_surface"; break;

		case reflection_on_surface: return "reflection_on_surface"; break;
		case fresnel_reflection_on_surface: return "fresnel_reflection_on_surface"; break;
		
		case refraction_to_outside: return "refraction_to_outside"; break;
		case refraction_to_inside: return "refraction_to_inside"; break;

		case scattering: return "scattering"; break;
    	default: return "unknown_interaction"; break;
	}
}
//------------------------------------------------------------------------------
const Intersection& RayForPropagation::get_final_intersection()const {
	return intersection_history->back();
}
//------------------------------------------------------------------------------
Vec3 RayForPropagation::get_final_intersection_incident_vector_in_object_frame()const {
	
	if(intersection_history->size() == 1) {//only production

		return Vec3::null;
	}else {

		const uint last_i = intersection_history->size() - 1 ;
		const uint second_last_i = last_i - 1;

		Vec3 final = 
			intersection_history->at(last_i).
				get_intersection_vector_in_world_system();
		
		Vec3 second_last = intersection_history->at(second_last_i).
				get_intersection_vector_in_world_system();

		Vec3 incident_direction_in_world = (final - second_last);
			incident_direction_in_world.normalize();
		
		return intersection_history->back().world2object()->
			get_transformed_orientation(incident_direction_in_world);
	}
}
//------------------------------------------------------------------------------
double RayForPropagation::get_time_of_flight()const {
	return 0.0;
}
//------------------------------------------------------------------------------
void RayForPropagation::transform(const HomTra3 *T) {
	support = T->get_transformed_position(support);
	direction = T->get_transformed_orientation(direction);	

	delete_history();	
	init_propagation_history();
}
//------------------------------------------------------------------------------