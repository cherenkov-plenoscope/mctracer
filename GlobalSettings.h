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
	//--------------------------------------------------------------------------
	// propagation options

	unsigned int number_of_max_reflections = 5;
		// A Ray, CameraRay or a Photon will only be traced until it
		// wasthis ofter reflected.

	bool flag_store_only_final_intersection = false;
		// Each Photon emitted has a history object storing its propagation
		// history. When flag_store_only_final_intersection is true only the
		// very last interaction, i.e. the absorbtion of the Photon will be 
		// stored in the history. 

	ColourProperties default_colour;
		// The default background colour in the raytracing images 

	//--------------------------------------------------------------------------
	// Multithread options

	bool multithread = true;
		// When multithread is true tmcTracer will use all threads availabe to
		// propagate the Ray,CameraRays,Photons.
		//
		// Pro: 
		//	Less computation time
		//
		// Contra: 
		//	The results can not be reproduced, althugh using the same 
		// 	seed for the random number generator. 


	//--------------------------------------------------------------------------
	// FreeOrbit life view options
	/*
	unsigned int virtual_reality_display_width_in_pixels = 320;
	unsigned int virtual_reality_display_hight_in_pixels = 240;
	*/

	// Randon number generator
	unsigned seed_for_random_number_generator_mt19937;
	// mt19937 is a standard mersenne_twister_engine
	//std::mt19937 pRNG_mt19937;

	// precision in csv table exports
	unsigned csv_decimal_presicion;
	bool flag_show_csv_row_identifier;
	bool flag_show_csv_row_identity;

public:
	//--------------------------------------------------------------------------
	GlobalSettings();
	//--------------------------------------------------------------------------
	bool StoreOnlyLastIntersection()const{
		return flag_store_only_final_intersection;
	};
	void SetStoreOnlyLastIntersection(bool flag){
		flag_store_only_final_intersection = flag;
	};
	//--------------------------------------------------------------------------
	bool MultiThread()const{ return multithread; }
	void SetMultiThread( bool flag ){ multithread = flag; }
	//--------------------------------------------------------------------------
	void set_max_number_of_reflections(const int new_max_number_of_reflections);
	//--------------------------------------------------------------------------
	void set_csv_decimal_presicion(const int new_csv_decimal_presicion);
	//--------------------------------------------------------------------------
	bool ShowCsvIdentifier()const;
	bool ShowCsvIdentity()const;
	void set_ShowCsvIdentity(bool show);
	//--------------------------------------------------------------------------
	ColourProperties get_default_colour()const;
	//--------------------------------------------------------------------------
	int get_max_number_of_reflections()const;
	//--------------------------------------------------------------------------
	unsigned get_decimal_precision_for_csv_output()const;
	//--------------------------------------------------------------------------
	void initialize_random_rumber_generator_seed_mt19937();
	//--------------------------------------------------------------------------
	unsigned get_seed_for_random_number_generator_mt19937()const;
	//--------------------------------------------------------------------------
	/*
	double Uniform(){
		return double( pRNG_mt19937() )/double( pRNG_mt19937.max() ); 
	};*/
};

#endif // __GLOBALSETTINGS_H_INCLUDED__ 
