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
#include "Cameras/SkyDome.h"
#include "Vector3D.h"
#include <string>
#include <sstream>

//=================================
class TracerSettings {
private:
	uint number_of_max_reflections;
		// A Ray will only be traced until the max 
		// number of reflections is reached.

	bool flag_store_only_final_intersection;
		// Each Photon emitted has a history object storing its propagation
		// history. When flag_store_only_final_intersection is true only the
		// very last interaction, i.e. the absorbtion of the Photon will be 
		// stored in the history. 

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

	// rendering
	const SkyDome::Dome* sky_dome;
public:
	struct Preview {
		uint cols;
		uint rows;
	} preview;

	struct Snapshot {
		uint cols;
		uint rows;
		uint rays_per_pixel;
	} snapshot;

	struct AperturCameraSettings {
		
	};

	bool sky_light_source;
	Vector3D global_light_direction;

	static const TracerSettings default_settings;
	TracerSettings();
	bool stores_only_last_intersection()const;
	void store_only_last_intersection(const bool flag);
	
	bool MultiThread()const;
	void SetMultiThread(const bool flag );	
	
	void set_max_number_of_reflections(const int new_max_number_of_reflections);
	int get_max_number_of_reflections()const;
	bool max_number_of_reflections_is_not_reached_yet(
		const uint reflection_counter
	)const;

	unsigned get_pseudo_random_number_seed()const;
	//Color get_default_color()const;

	void set_sky_dome(const SkyDome::Dome* dome);
	const SkyDome::Dome* get_sky_dome()const;
};

#endif // __TracerSettings_H_INCLUDED__ 
