//=================================
// include guard
#ifndef __TracerSettings_H_INCLUDED__
#define __TracerSettings_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Color.h"
#include "TracerException.h"
#include <string>
#include <sstream>

//=================================
class TracerSettings {
private:
	//--------------------------------------------------------------------------
	// propagation options

	uint number_of_max_reflections;
		// A Ray, CameraRay or a Photon will only be traced until the max 
		// number of reflections is reached.

	bool flag_store_only_final_intersection;
		// Each Photon emitted has a history object storing its propagation
		// history. When flag_store_only_final_intersection is true only the
		// very last interaction, i.e. the absorbtion of the Photon will be 
		// stored in the history. 

	Color default_color;
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

	unsigned pseudo_random_number_seed;
public:
	static const TracerSettings default_settings;
	TracerSettings();
	bool StoreOnlyLastIntersection()const;
	void SetStoreOnlyLastIntersection(const bool flag);
	bool MultiThread()const;
	void SetMultiThread(const bool flag );	
	void set_max_number_of_reflections(const int new_max_number_of_reflections);
	Color get_default_color()const;
	int get_max_number_of_reflections()const;
	bool max_number_of_reflections_is_not_reached_yet(
		const uint reflection_counter
	)const;
	void store_only_final_intersection();
	unsigned get_pseudo_random_number_seed()const;
};

#endif // __TracerSettings_H_INCLUDED__ 
