#include "Photon.h"
//======================================================================
Photon::Photon(	double supX,double supY,double supZ,
				double dirX,double dirY,double dirZ,
				double new_wavelength){
	wavelength = new_wavelength;
	support.set(supX,supY,supZ);
	direction.set(dirX,dirY,dirZ);
}
//======================================================================
Photon::Photon(Vector3D new_support, Vector3D new_direction, double new_wavelength){
	support = new_support; 
	direction  = new_direction;
	wavelength = new_wavelength;
}
//======================================================================
Photon::Photon(Ray prototype_ray_for_photon,double new_wavelength){
	wavelength = new_wavelength;
	support = prototype_ray_for_photon.Support();
	direction  = prototype_ray_for_photon.Direction();
}
//======================================================================
Photon::Photon(double new_wavelength){
	wavelength = new_wavelength;
}
//======================================================================
double Photon::get_wavelength()const{
	return wavelength;
}
//======================================================================
std::string Photon::get_string()const{
	std::stringstream out; 
	out << Ray::get_string() << ", wavelength: ";
	out << wavelength*1e9    << "[nm]";
	return out.str();
}
//======================================================================
void Photon::disp()const{
	std::cout << "Photon -> " << get_string() << "\n";
}
//======================================================================
CsvRow Photon::getPhotonCsvRow(GlobalSettings& settings)const{

	CsvRow row;

	stringstream out;
	out.precision(settings.get_decimal_precision_for_csv_output());

	out << wavelength;
	row.push_back( out.str() );
	out.str("");

	return row;
}
//======================================================================
CsvRow Photon::getCsvRow(GlobalSettings& settings)const{
	CsvRow combinedRow;
	
	if(settings.ShowCsvIdentifier())
	combinedRow.push_back("Pho");

	combinedRow.append( getRayCsvRow(settings) );
	combinedRow.append( getPhotonCsvRow(settings) );

	return combinedRow;
}
//======================================================================
// friends of osstream
//======================================================================
std::ostream& operator<<(std::ostream& os, 
const Photon& photon_to_be_displayed){
    os << photon_to_be_displayed.get_string();
    return os;
}
//======================================================================
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
//======================================================================
void Photon::propagate(	
	const CartesianFrame* world, 
	ListOfInteractions* history,
	int interaction_count,
	const CartesianFrame* object_propagated_from,
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator* dice
){

	//std::cout << interaction_count <<endl;
	//==================================================================
	// claculate a list/vector containing all possible intersection
	// candidates using pre-trace
	//==================================================================
	std::vector<const CartesianFrame*> ListOfObjectsWhichMightIntersect;
	find_intersection_candidates_in_tree_of_frames( world , &ListOfObjectsWhichMightIntersect );

	//==================================================================
	// calculate a list of objects wich _do_ intersect from the list_of_
	// objects_which_might_intersect
	//==================================================================

	//std::vector<const CartesianFrame*> ListOfIntersectingObjects;
	//std::vector<Intersection*> VecOfPtr2Intersections_which_might_take_place;
	std::vector<Intersection*> VecOfPtr2Intersections;
	
	find_intersections_in_intersection_candidate_frames(
		&ListOfObjectsWhichMightIntersect,
		&VecOfPtr2Intersections,
		object_propagated_from//,
		//interaction_count
	);

	//==================================================================
	// test wether there is at least one object intersecting the ray or
	// not by using the size of the ListOfIntersectingObjects
	//==================================================================
	
	Intersection *ClosestIntersection = NULL;

	if(VecOfPtr2Intersections.size()==0){

		// There is no intersection taking place

	}else{
		//==============================================================
		// find the closest object in the list of 
		// ListOfIntersectingObjects
		//==============================================================

		ClosestIntersection = calculate_closest_intersection(
			&VecOfPtr2Intersections
		);

		//==============================================================
		// test reflection of the object ClosestIntersection is
		// pointing to
		//==============================================================
		if(

		ClosestIntersection->
		get_pointer_to_intersecting_object()->
		get_hit_reflection_flag()

		&& 

		interaction_count < 
		(settings->get_max_number_of_reflections() - 1)

		){	
			//==========================================================
			// The object ClosestIntersection is pointing to
			// does reflect and there are still reflections left to be 
			// performed. We now check whether the photon survives 
			// this reflection or it gets absorbed here.
			//==========================================================

			if(	

			dice->uniform() 
			<=
			ClosestIntersection->
			get_pointer_to_intersecting_object()->
			get_ptr2_reflection()->
			ReflectionCoefficient(wavelength)

			){
				// reflection takes place

				interaction_count++;
				
				//==========================================================
				// calculate the ray reflected by the object
				// ClosestIntersection is pointing to relativ to
				// the world coordinate system
				//==========================================================
				Photon ReflectedPhoton(wavelength);

				calculate_reflected_ray(
					ClosestIntersection,
					&ReflectedPhoton
				);
				
				if( !settings->StoreOnlyLastIntersection() ){
					history->Interactions.push_back(ClosestIntersection);
				}
				
				ReflectedPhoton.propagate(
					world,
					history,
					interaction_count,
					ClosestIntersection->
					get_pointer_to_intersecting_object(),
					settings,
					dice
				);

			}else{
				// There is no reflection at all. The Photon is absorbed completley
				// store final intersection
				history->Interactions.push_back(ClosestIntersection);
			}

		}else{
			// There is no reflection at all. The Photon is absorbed completley
			// store final intersection
			history->Interactions.push_back(ClosestIntersection);
		}	
	}
	//=============
	//free memory VecOfPtr2Intersections 
	//=============
	for(
		Intersection *ptr_to_intersection : 
		VecOfPtr2Intersections
	){
		// delete all intersections which are not needed any longer
		if(
			ptr_to_intersection != ClosestIntersection 
			&& 
			settings->StoreOnlyLastIntersection()
		)
		delete ptr_to_intersection;
	}
	
}