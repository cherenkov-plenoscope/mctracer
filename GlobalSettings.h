//=================================
// include guard
#ifndef __GLOBALSETTINGS_H_INCLUDED__
#define __GLOBALSETTINGS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "ColourProperties.h"
#include <chrono>

//=================================
class GlobalSettings {
private:
	int number_of_max_reflections;
	ColourProperties default_colour;
	unsigned seed_for_random_number_generator_mt19937;
public:
//======================================================================
GlobalSettings();
//======================================================================
bool set_max_number_of_reflections
(const int new_max_number_of_reflections);
//======================================================================
ColourProperties get_default_colour()const;
//======================================================================
int get_max_number_of_reflections()const;
//======================================================================
void initialize_random_rumber_generator_seed_mt19937();
//======================================================================
unsigned get_seed_for_random_number_generator_mt19937()const;
};

#endif // __GLOBALSETTINGS_H_INCLUDED__ 
