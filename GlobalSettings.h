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
#include "TracerException.h"
#include <string>
#include <sstream>

//=================================
class GlobalSettings {
private:
	int number_of_max_reflections;

	// the default background colour in the raytracing images 
	ColourProperties default_colour;

	// Randon number generator
	unsigned seed_for_random_number_generator_mt19937;

	// precision in csv table exportation
	unsigned csv_decimal_presicion;
	bool flag_show_csv_row_identifier;

	// photon propagation
	bool flag_store_only_final_intersection;

public:
//======================================================================
GlobalSettings();
bool StoreOnlyLastIntersection()const;
//======================================================================
void set_max_number_of_reflections(const int new_max_number_of_reflections);
//======================================================================
void set_csv_decimal_presicion(const int new_csv_decimal_presicion);
//======================================================================
bool ShowCsvIdentifier()const;
//======================================================================
ColourProperties get_default_colour()const;
//======================================================================
int get_max_number_of_reflections()const;
//======================================================================
unsigned get_decimal_precision_for_csv_output()const;
//======================================================================
void initialize_random_rumber_generator_seed_mt19937();
//======================================================================
unsigned get_seed_for_random_number_generator_mt19937()const;
};

#endif // __GLOBALSETTINGS_H_INCLUDED__ 
