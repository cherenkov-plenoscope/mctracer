#include "Photon.h"
//======================================================================
Photon::Photon(	double baseX,double baseY,double baseZ,
				double dirX,double dirY,double dirZ,
				double new_wavelength){
	wavelength = new_wavelength;
	base.set(baseX,baseY,baseZ);
	dir.set(dirX,dirY,dirZ);
}
//======================================================================
Photon::Photon(Ray prototype_ray_for_photon,double new_wavelength){
	wavelength = new_wavelength;
	base = prototype_ray_for_photon.get_support();
	dir  = prototype_ray_for_photon.get_direction();
}
//======================================================================
std::string Photon::get_string()const{
	std::stringstream out; 
	out << Ray::get_string() << ", wavelength: ";
	out << wavelength*1e9 << "[nm]";
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

	CsvRow RowOfRay 	= getRayCsvRow(settings);
	CsvRow RowOfPhoton 	= getPhotonCsvRow(settings);

	combinedRow.append(RowOfRay);
	combinedRow.append(RowOfPhoton);

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
void Photon::propagate(	const CartesianFrame* world, 
						int interaction_count,
						const CartesianFrame* object_reflected_from,
						const GlobalSettings* settings){
	
	//==================================================================
	// claculate a list/vector containing all possible intersection
	// candidates using pre-trace
	//==================================================================
	std::vector<const CartesianFrame*> list_of_objects_which_might_intersect;
	pre_trace( world , &list_of_objects_which_might_intersect );
	
	//==================================================================
	// calculate a list of objects wich _do_ intersect from the list_of_
	// objects_which_might_intersect
	//==================================================================

	std::vector<const CartesianFrame*> list_of_intersecting_objects;
	std::vector<Intersection*> list_of_ptr_to_intersections_which_might_take_place;
	std::vector<Intersection*> list_of_ptr_to_intersections;
	
	test_intersection_for_hit_candidates(
		&list_of_objects_which_might_intersect,
		&list_of_intersecting_objects,
		&list_of_ptr_to_intersections_which_might_take_place,
		&list_of_ptr_to_intersections,
		object_reflected_from,
		interaction_count
	);

	//==================================================================
	// test wether there is at least one object intersecting the ray or
	// not by using the size of the list_of_intersecting_objects
	//==================================================================
	
	//ColourProperties colour_to_return;
	
	if(list_of_ptr_to_intersections.size()==0)
	{
		// There is no intersection taking place

	}else{
		//==============================================================
		// find the closest object in the list of 
		// list_of_intersecting_objects
		//==============================================================
		
		Intersection *ptr_to_closest_intersection;
		ptr_to_closest_intersection = 
		calculate_closest_intersection(
			ptr_to_closest_intersection,
			&list_of_ptr_to_intersections
		);
		
		//==============================================================
		// test reflection of the object pointer_to_closest_frame is
		// pointing to
		//==============================================================
		if(

		ptr_to_closest_intersection->
		get_pointer_to_intersecting_object()
		->get_hit_reflection_flag()

		&& 

		interaction_count < settings->get_max_number_of_reflections()

		){	
			//==========================================================
			// the object pointer_to_closest_frame is pointing to
			// does reflect, so we increase the reflection counter
			//==========================================================
			
			interaction_count++;
			
			//==========================================================
			// calculate the ray reflected by the object
			// pointer_to_closest_frame is pointing to relativ to
			// the world coordinate system
			//==========================================================
			Ray ray_reflection_on_object;

			calculate_reflected_ray(
				ptr_to_closest_intersection,
				&ray_reflection_on_object
			);
			
			if( !settings->StoreOnlyLastIntersection() ){
				PhotonPropagationHistory.push_back(ptr_to_closest_intersection);
			}

			propagate(
			world,
			interaction_count,
			ptr_to_closest_intersection->
			get_pointer_to_intersecting_object(),
			settings
			);

		}else{
			
			// There is no reflection at all. The Photon is absorbed completley
			// store final intersection
			PhotonPropagationHistory.push_back(ptr_to_closest_intersection);

		}	
	}
	//=============
	//free memory list_of_ptr_to_intersections 
	//=============
	for (Intersection *ptr_to_intersection : 
	list_of_ptr_to_intersections_which_might_take_place ) {
		
		delete ptr_to_intersection;
		
	}
}