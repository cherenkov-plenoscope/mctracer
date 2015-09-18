#include "TracerSettings.h"
//------------------------------------------------------------------------------
TracerSettings::TracerSettings() {
	default_colour.set_RGB_0to255(128,128,128);

	flag_store_only_final_intersection = false;

	number_of_max_reflections = 5;
}
//------------------------------------------------------------------------------
void TracerSettings::set_max_number_of_reflections(
	const int new_max_number_of_reflections
) {
	if( new_max_number_of_reflections>0 ){

		number_of_max_reflections = new_max_number_of_reflections;

	}else{
		std::stringstream out;
		out << "TracerSettings::set_max_number_of_reflections\n";
		out << "The number_of_max_reflections in the Settings ";
		out << "must not be zero or negative!\n";
		out << "Actual it is " << new_max_number_of_reflections << "\n";
		throw TracerException(out.str());
	}
}
//------------------------------------------------------------------------------
bool TracerSettings::max_number_of_reflections_is_not_reached_yet(
	const uint reflection_counter
)const {
	return reflection_counter <= number_of_max_reflections;
}
//------------------------------------------------------------------------------
Color TracerSettings::get_default_colour()const {
	return default_colour;
}
//------------------------------------------------------------------------------
int TracerSettings::get_max_number_of_reflections()const {
	return number_of_max_reflections;
}
//------------------------------------------------------------------------------
bool TracerSettings::StoreOnlyLastIntersection()const {
	return flag_store_only_final_intersection;
}
//------------------------------------------------------------------------------
void TracerSettings::SetStoreOnlyLastIntersection(bool flag) {
	flag_store_only_final_intersection = flag;
}
//------------------------------------------------------------------------------
bool TracerSettings::MultiThread()const { return multithread; }
//------------------------------------------------------------------------------
void TracerSettings::SetMultiThread( bool flag ) { multithread = flag; }
//------------------------------------------------------------------------------
void TracerSettings::store_only_final_intersection() {
	flag_store_only_final_intersection = true;
}