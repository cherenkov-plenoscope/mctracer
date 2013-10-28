#include "GlobalSettings.h"
//======================================================================
GlobalSettings::GlobalSettings(){
	default_colour.set_colour_0to255(128,128,128);
	number_of_max_reflections = 5;
	initialize_random_rumber_generator_seed_mt19937();
}
//======================================================================
bool GlobalSettings::set_max_number_of_reflections
(const int new_max_number_of_reflections){
	if( new_max_number_of_reflections>0 )
	{
		number_of_max_reflections=new_max_number_of_reflections;
		return true;
	}else{
		std::cout<<"global_settings->max_number_of_reflections";
		std::cout<<new_max_number_of_reflections;
		std::cout<<" must not be negativ!"<<std::endl;
		return false;
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

