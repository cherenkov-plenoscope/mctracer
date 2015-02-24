#include "Photon.h"
//------------------------------------------------------------------------------
Photon::Photon(
	const Vector3D support,
	const Vector3D direction,
	const double wavelength
) {
	SetRay(support, direction);
	this->wavelength = wavelength;
	init_propagation_history();	
}
//------------------------------------------------------------------------------
Photon::Photon(
	const Photon* photon_to_be_carried_on
) {
	expand_propagation_properties_of_ray(photon_to_be_carried_on);
	wavelength = photon_to_be_carried_on->wavelength;
}
//------------------------------------------------------------------------------
double Photon::get_wavelength()const {
	return wavelength;
}
//------------------------------------------------------------------------------
std::string Photon::get_print()const {
	std::stringstream out; 
	out << get_ray_print() << ", wavelength: " << wavelength*1e9 << "nm";
	return out.str();
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os,const Photon& photon_to_be_displayed){
    os << photon_to_be_displayed.get_print();
    return os;
}
//------------------------------------------------------------------------------
void Photon::propagate_in(PropagationEnvironment* environment) {
	
	Intersection* first_intersection = 
		get_first_intersection_in(environment->world_geometry);

	if(first_intersection->does_intersect()) {
		interaction_with_object(first_intersection);
	}else{
		absorbtion_in_void_space(first_intersection);
	}	
}
//------------------------------------------------------------------------------
void Photon::interaction_with_object(const Intersection* intersection) {

	InteractionSurfaceFinder surface_finder(this, intersection);

	if(surface_finder.has_surface_interaction()) {
		surface_finder.get_interaction_surface();
		push_back_to_propagation_history(intersection);	
	}else{

	}
}
//------------------------------------------------------------------------------
void Photon::absorbtion_in_void_space(const Intersection* intersec) {
	//push_back_to_propagation_history(intersec);	
}
//------------------------------------------------------------------------------
/*void Photon::propagate_further_on_and_ignore_intersection() {
	Photon photon_extension(this);
	photon_extension
}*/