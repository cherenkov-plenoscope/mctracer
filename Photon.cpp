#include "Photon.h"
//------------------------------------------------------------------------------
Photon::Photon(Vector3D new_support, Vector3D new_direction, double new_wavelength){
	support = new_support; 
	direction  = new_direction;
	direction.normalize();
	wavelength = new_wavelength;
}
//------------------------------------------------------------------------------
Photon::Photon(Ray prototype_ray_for_photon,double new_wavelength){
	wavelength = new_wavelength;
	support = prototype_ray_for_photon.Support();
	direction  = prototype_ray_for_photon.Direction();
}
//------------------------------------------------------------------------------
Photon::Photon(double new_wavelength){
	wavelength = new_wavelength;
}
//------------------------------------------------------------------------------
double Photon::get_wavelength()const{
	return wavelength;
}
//------------------------------------------------------------------------------
std::string Photon::get_print()const{
	std::stringstream out; 
	out << get_ray_print() << ", wavelength: " << wavelength*1e9    << "nm";
	return out.str();
}
//------------------------------------------------------------------------------
void Photon::print()const{
	std::cout << "Photon -> " << get_print() << "\n";
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os,const Photon& photon_to_be_displayed){
    os << photon_to_be_displayed.get_print();
    return os;
}
//------------------------------------------------------------------------------
void Photon::propagate(
	const CartesianFrame* world, 
	ListOfInteractions* history,
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator* dice
){
	int interaction_count = 0;

	propagate(
		world,
		history,
		interaction_count,
		settings,
		dice
	);
}
//------------------------------------------------------------------------------
bool Photon::reflection_takes_place_in_intesection(
	Intersection* intersection, PseudoRandomNumberGenerator* dice
) {
	return dice->uniform() <= intersection->
		get_intersecting_object()->
			get_reflection_properties()->
				ReflectionCoefficient(wavelength);
}
//------------------------------------------------------------------------------
void Photon::propagate(	
	const CartesianFrame* world, 
	ListOfInteractions* history,
	uint interaction_count,
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator* dice
) {
	Intersection* intersection = get_first_intersection_in(world);

	if(intersection->does_intersect()) {

		if(	
			intersection->get_intersecting_object()->does_reflect() &&
			settings->max_number_of_reflections_is_not_reached_yet(interaction_count)
		) {

			reflect_on_surface(
				world,
				history,
				interaction_count,
				settings,
				dice,
				intersection
			);

		}else if(
			intersection->get_intersecting_object()->does_refract() &&
			settings->max_number_of_reflections_is_not_reached_yet(interaction_count)
		) {

			refract_on_surface(
				world, 
				history,
				interaction_count,
				settings,
				dice,
				intersection
			);

		}else{
			// absorbed
			history->Interactions.push_back(intersection);
		}	
	}
}
//------------------------------------------------------------------------------
void Photon::reflect_on_surface(
	const CartesianFrame* world, 
	ListOfInteractions* history,
	uint interaction_count,
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator* dice,
	Intersection* intersection
) {
	// The object intersection is pointing to
	// does reflect and there are still reflections left to be 
	// performed. We now check whether the photon survives 
	// this reflection or it gets absorbed here.

	if(reflection_takes_place_in_intesection(intersection, dice)) {
		
		interaction_count++;
		
		// calculate the ray reflected by the object
		// intersection is pointing to relativ to
		// the world coordinate system

		Photon ReflectedPhoton(wavelength);
		calculate_reflected_ray(
			intersection,
			&ReflectedPhoton
		);
		
		if( !settings->StoreOnlyLastIntersection() )
			history->Interactions.push_back(intersection);
		
		ReflectedPhoton.propagate(
			world,
			history,
			interaction_count,
			settings,
			dice
		);
	}else{
		// reflection took not place by chance, absorbtion
		history->Interactions.push_back(intersection);
	}		
}
//------------------------------------------------------------------------------
void Photon::refract_on_surface(
	const CartesianFrame* world, 
	ListOfInteractions* history,
	uint interaction_count,
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator* dice,
	Intersection* intersection
) {
	interaction_count++;

	// refraction
	Photon refracted_photon(
		intersection->get_intersection_vector_in_world_system(),
		direction,
		wavelength
	);

	if( !settings->StoreOnlyLastIntersection() )
		history->Interactions.push_back(intersection);

	refracted_photon.propagate(
		world,
		history,
		interaction_count,
		settings,
		dice
	);	
}
//------------------------------------------------------------------------------