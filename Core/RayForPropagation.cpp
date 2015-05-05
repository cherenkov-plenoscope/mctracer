#include "RayForPropagation.h"
//------------------------------------------------------------------------------
RayForPropagation::RayForPropagation() {
}
//------------------------------------------------------------------------------
RayForPropagation::RayForPropagation(
	const Vector3D support,
	const Vector3D direction
) {
	SetRay(support, direction);
	init_propagation_history();
}
//------------------------------------------------------------------------------
void RayForPropagation::init_propagation_history() {
	intersection_history = new std::vector<const Intersection*>;	
	interaction_type_history = new std::vector<InteractionType>;

	push_back_production_of_ray();
}
//------------------------------------------------------------------------------
void RayForPropagation::push_back_production_of_ray() {

	Intersection* production_intersection = new Intersection(
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
RayForPropagation::~RayForPropagation() {
	//std::cout << "ohh\n";
	//delete intersection_history;
	//delete interaction_type_history;
}
//------------------------------------------------------------------------------
void RayForPropagation::delete_history() {

	for(uint i=0; i<intersection_history->size(); i++)
		delete intersection_history->at(i);
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
std::string RayForPropagation::get_print()const {
	std::stringstream out;
	out << get_ray_print() << ", " << get_rayforpropagation_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string RayForPropagation::get_history_print()const {
	std::stringstream out;
	int index = 0;
	for(InteractionType type : *interaction_type_history) {
		out << ++index << ") " << get_type_print(type) << " in ";
		out << intersection_history->at(index-1)->
			get_intersecting_object()->get_name_of_frame();
		out << " " << intersection_history->at(index-1)->	
			get_intersection_vector_in_world_system() << ", dist to prev.:";

		if(index>1) {
			out << intersection_history->at(index-1)->get_intersection_vector_in_world_system().distance_to(
			   		intersection_history->at(index-2)->get_intersection_vector_in_world_system()
				)*1e9 << "nm";
			}

		out << "\n";
	}
	return out.str();
}
//------------------------------------------------------------------------------
std::string RayForPropagation::get_rayforpropagation_print()const {
	std::stringstream out;
	out << "ID: " << identifier_number << ", Interactions: " ;
	out << get_number_of_interactions_so_far() << "\n";
	out << get_history_print();
	return out.str();	
}
//------------------------------------------------------------------------------
void RayForPropagation::push_back_intersection_and_type_to_propagation_history(
	const Intersection* interact, 
	const InteractionType type
) {
	intersection_history->push_back(interact);
	interaction_type_history->push_back(type);	
}
//------------------------------------------------------------------------------
double RayForPropagation::get_accumulative_distance()const {
  	
  	double accumulative_distance = 0.0;
	
	for(const Intersection* intersection : *intersection_history) 
      	accumulative_distance += intersection->get_intersection_distance();

    return accumulative_distance;	
}
//------------------------------------------------------------------------------
std::ostream& operator<<(
	std::ostream& os, 
	const RayForPropagation& ray_to_be_displayed
) {
    os << ray_to_be_displayed.get_print();
    return os;	
}
//------------------------------------------------------------------------------
void RayForPropagation::propagate_in(PropagationEnvironment* env) {
	throw TracerException("Prototype RayForPropagation must not be propagated itself, only its children such as the photon are meant to be propagated.");
}
//------------------------------------------------------------------------------
uint RayForPropagation::get_number_of_interactions_so_far()const {
	return uint(intersection_history->size());
}
//------------------------------------------------------------------------------
const Intersection* RayForPropagation::get_intersection_at(
	const uint index
)const{
	return intersection_history->at(index);
}
//------------------------------------------------------------------------------
std::string RayForPropagation::get_type_print(const InteractionType type)const {
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
const Intersection* RayForPropagation::get_final_intersection()const {
	return intersection_history->back();
}
//------------------------------------------------------------------------------
double RayForPropagation::get_time_of_flight()const {
	return 0.0;
}
//------------------------------------------------------------------------------