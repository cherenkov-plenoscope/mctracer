#include "RayForPropagation.h"
//------------------------------------------------------------------------------
RayForPropagation::RayForPropagation() {}
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
	propagation_history = new std::vector<const Intersection*>;	
}
//------------------------------------------------------------------------------
RayForPropagation::RayForPropagation(
	const RayForPropagation* ray_to_be_expanded
) {
	expand_propagation_properties_of_ray(ray_to_be_expanded);
}
//------------------------------------------------------------------------------
void RayForPropagation::expand_propagation_properties_of_ray(
	const RayForPropagation* ray_to_be_expanded
) {
	identifier_number = ray_to_be_expanded->identifier_number;
	propagation_history = ray_to_be_expanded->propagation_history;	
}
//------------------------------------------------------------------------------
RayForPropagation::~RayForPropagation() {
	delete_propagation_history();
}
//------------------------------------------------------------------------------
void RayForPropagation::delete_propagation_history() {
	for(const Intersection* intersec: *propagation_history)
		delete intersec;
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
const std::vector<const Intersection*>* RayForPropagation::get_propagation_history()const {
	return propagation_history;
}
//------------------------------------------------------------------------------
std::string RayForPropagation::get_print()const {
	std::stringstream out;
	out << get_ray_print() << ", " << get_rayforpropagation_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string RayForPropagation::get_rayforpropagation_print()const {
	std::stringstream out;
	out << "ID: " << identifier_number << ", Interactions: " ;
	out << propagation_history->size();
	return out.str();	
}
//------------------------------------------------------------------------------
void RayForPropagation::push_back_to_propagation_history(
	const Intersection* intersec
) {
	propagation_history->push_back(intersec);
}
//------------------------------------------------------------------------------
double RayForPropagation::get_accumulative_distance()const {
  	
  	double accumulative_distance = 0.0;
	
	for(const Intersection* intersec : *propagation_history) 
      	accumulative_distance += intersec->get_intersection_distance();

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
	std::cout << "screw u\n";
	//throw TracerException("Prototype RayForPropagation must not be propagated itself, only its children such as he photon are meant to be propagated.");
}