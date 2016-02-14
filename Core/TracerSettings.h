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
#include "Cameras/VisualConfig.h"
#include "Vector3D.h"
#include <string>
#include <sstream>

//=================================
struct TracerSettings {

	uint max_number_of_interactions_per_photon;
	bool store_only_final_intersection;
	bool use_multithread_when_possible;
	uint pseudo_random_number_seed;

	VisualConfig visual;

	static const TracerSettings default_settings;
	TracerSettings();	
};
#endif // __TracerSettings_H_INCLUDED__ 
