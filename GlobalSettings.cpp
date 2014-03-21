#include "GlobalSettings.h"
//======================================================================
GlobalSettings::GlobalSettings(){
	default_colour.set_colour_0to255(128,128,128);
	number_of_max_reflections = 5;
	initialize_random_rumber_generator_seed_mt19937();
	flag_show_csv_row_identifier = true;
	flag_store_only_final_intersection = false;
}
//======================================================================
void GlobalSettings::set_max_number_of_reflections(
	const int new_max_number_of_reflections){
	if( new_max_number_of_reflections>0 ){

		number_of_max_reflections=new_max_number_of_reflections;

	}else{

		std::stringstream out;
		out << "The number_of_max_reflections in the Settings ";
		out << "must not be zero or negative!\n";
		out << "Actual it is " << new_max_number_of_reflections << "\n";

		throw BadValue(
			"Settings -> set_max_number_of_reflections()",
			"number_of_max_reflections",
			out.str());
	}
}
//======================================================================
void GlobalSettings::set_csv_decimal_presicion(
	const int new_csv_decimal_presicion){
	if( new_csv_decimal_presicion>0 ){

		csv_decimal_presicion = unsigned(new_csv_decimal_presicion);

	}else{

		std::stringstream out;
		out << "The csv_decimal_presicion for cvs output in the Settings ";
		out << "must not be zero or negative!\n";
		out << "Actual it is " << new_csv_decimal_presicion << "\n";

		throw BadValue(
			"Settings -> set_csv_decimal_presicion_in_csv_output()",
			"csv_decimal_presicion",
			out.str());
	}
}
//======================================================================
ColourProperties GlobalSettings::get_default_colour()const{
	return default_colour;
}
//======================================================================
int GlobalSettings::get_max_number_of_reflections()const{
	return number_of_max_reflections;
}
//======================================================================
unsigned GlobalSettings::get_decimal_precision_for_csv_output()const{
	return csv_decimal_presicion;
}
//======================================================================
void GlobalSettings::initialize_random_rumber_generator_seed_mt19937(){
	// Mersenne Twister 1997 by Matsumoto and Nishimura
	// obtain a seed from the system clock:
	seed_for_random_number_generator_mt19937 = 
	std::chrono::system_clock::now().time_since_epoch().count();
}
//======================================================================
unsigned GlobalSettings::get_seed_for_random_number_generator_mt19937()
const{
	return seed_for_random_number_generator_mt19937;
}
//======================================================================
bool  GlobalSettings::ShowCsvIdentifier()const{
	return flag_show_csv_row_identifier;
}
//======================================================================
bool GlobalSettings::StoreOnlyLastIntersection()const{
	return flag_store_only_final_intersection;
}