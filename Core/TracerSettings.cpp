#include "TracerSettings.h"
#include "Tools/Tools.h"

const TracerSettings TracerSettings::default_settings = TracerSettings();
//------------------------------------------------------------------------------
TracerSettings::TracerSettings() {

	default_color = Color::sky_blue;
	flag_store_only_final_intersection = false;
	number_of_max_reflections = 5;
	pseudo_random_number_seed = 0;
	sky_dome = &SkyDome::blue_sky;
}
//------------------------------------------------------------------------------
void TracerSettings::set_max_number_of_reflections(
	const int new_max_number_of_reflections
) {
	AssertionTools::value_with_name_is_greater_zero_given_context(
		new_max_number_of_reflections, "max number of reflections",
		"TracerSettings::set_max_number_of_reflections\n"
		"The number_of_max_reflections in the Settings "
		"must not be zero or negative!"
	);
	
	number_of_max_reflections = new_max_number_of_reflections;
}
//------------------------------------------------------------------------------
bool TracerSettings::max_number_of_reflections_is_not_reached_yet(
	const uint reflection_counter
)const {
	return reflection_counter <= number_of_max_reflections;
}
//------------------------------------------------------------------------------
Color TracerSettings::get_default_color()const {
	return default_color;
}
//------------------------------------------------------------------------------
int TracerSettings::get_max_number_of_reflections()const {
	return number_of_max_reflections;
}
//------------------------------------------------------------------------------
bool TracerSettings::stores_only_last_intersection()const {
	return flag_store_only_final_intersection;
}
//------------------------------------------------------------------------------
void TracerSettings::store_only_last_intersection(const bool flag) {
	flag_store_only_final_intersection = flag;
}
//------------------------------------------------------------------------------
bool TracerSettings::MultiThread()const { 
	return multithread; 
}
//------------------------------------------------------------------------------
void TracerSettings::SetMultiThread(const bool flag ) {
	multithread = flag; 
}
//------------------------------------------------------------------------------
unsigned TracerSettings::get_pseudo_random_number_seed()const {
	return pseudo_random_number_seed;
}
//------------------------------------------------------------------------------
const SkyDome::Dome* TracerSettings::get_sky_dome()const {
	return sky_dome;
}
//------------------------------------------------------------------------------
void TracerSettings::set_sky_dome(const SkyDome::Dome* dome) {
	sky_dome = dome;
}