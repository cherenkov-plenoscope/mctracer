#include "Photon.h"
//------------------------------------------------------------------------------
Photon::Photon(	double supX,double supY,double supZ,
				double dirX,double dirY,double dirZ,
				double new_wavelength){
	wavelength = new_wavelength;
	support.set(supX,supY,supZ);
	direction.set(dirX,dirY,dirZ);
	direction.normalize();
}
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
std::string Photon::get_string()const{
	std::stringstream out; 
	out << Ray::get_string() << ", wavelength: ";
	out << wavelength*1e9    << "[nm]";
	return out.str();
}
//------------------------------------------------------------------------------
void Photon::disp()const{
	std::cout << "Photon -> " << get_string() << "\n";
}
//------------------------------------------------------------------------------
// friends of osstream
std::ostream& operator<<(std::ostream& os,const Photon& photon_to_be_displayed){
    os << photon_to_be_displayed.get_string();
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
	const CartesianFrame* object_propagated_from = NULL;

	propagate(
		world,
		history,
		interaction_count,
		object_propagated_from,
		settings,
		dice
	);

}
//------------------------------------------------------------------------------
bool Photon::reflection_takes_place_in_intesection(
	Intersection* intersection, PseudoRandomNumberGenerator* dice
) {
	return dice->uniform() <= intersection->
		get_pointer_to_intersecting_object()->
			get_reflection_properties()->
				ReflectionCoefficient(wavelength);
}
//------------------------------------------------------------------------------
void Photon::propagate(	
	const CartesianFrame* world, 
	ListOfInteractions* history,
	int interaction_count,
	const CartesianFrame* object_propagated_from,
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator* dice
){
	std::vector<const CartesianFrame*> objects_which_might_intersect;
	find_intersection_candidates_in_tree_of_frames(
		world,
	 	&objects_which_might_intersect
	);

	std::vector<Intersection*> intersections;
	find_intersections_in_intersection_candidate_frames(
		&objects_which_might_intersect,
		&intersections,
		object_propagated_from
	);

	//==================================================================
	// test wether there is at least one object intersecting the ray or
	// not by using the size of the ListOfIntersectingObjects
	//==================================================================
	
	Intersection *closest_intersection = NULL;

	if(intersections.size() > 0) {
		//==============================================================
		// find the closest object in the list of 
		// ListOfIntersectingObjects
		//==============================================================

		closest_intersection = calculate_closest_intersection(
			&intersections
		);

		//==============================================================
		// test reflection of the object closest_intersection is
		// pointing to
		//==============================================================
		if(
			closest_intersection->
			get_pointer_to_intersecting_object()->get_reflection_flag()
			&& 
			interaction_count < (settings->get_max_number_of_reflections() - 1)
		) {	
			//==========================================================
			// The object closest_intersection is pointing to
			// does reflect and there are still reflections left to be 
			// performed. We now check whether the photon survives 
			// this reflection or it gets absorbed here.
			//==========================================================

			if(reflection_takes_place_in_intesection(closest_intersection,dice)){
				
				interaction_count++;
				
				//==========================================================
				// calculate the ray reflected by the object
				// closest_intersection is pointing to relativ to
				// the world coordinate system
				//==========================================================
				Photon ReflectedPhoton(wavelength);

				calculate_reflected_ray(
					closest_intersection,
					&ReflectedPhoton
				);
				
				if( !settings->StoreOnlyLastIntersection() ) {
					history->Interactions.push_back(closest_intersection);
				}
				
				ReflectedPhoton.propagate(
					world,
					history,
					interaction_count,
					closest_intersection->
					get_pointer_to_intersecting_object(),
					settings,
					dice
				);

			}else{
				// There is no reflection at all. The Photon is absorbed completley
				// store final intersection
				history->Interactions.push_back(closest_intersection);
			}

		}else{
			// There is no reflection at all. The Photon is absorbed completley
			// store final intersection
			history->Interactions.push_back(closest_intersection);
		}	
	}
	//=============
	//free memory intersections 
	//=============
	for( Intersection *ptr_to_intersection : intersections ) {
		// delete all intersections which are not needed any longer
		if(
			ptr_to_intersection != closest_intersection 
			&& 
			settings->StoreOnlyLastIntersection()
		)
		delete ptr_to_intersection;
	}
}