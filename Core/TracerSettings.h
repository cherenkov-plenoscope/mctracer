//=================================
// include guard
#ifndef __TracerSettings_H_INCLUDED__
#define __TracerSettings_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Color.h"
#include "Core/TracerException.h"
#include "Vec3.h"
#include <string>
#include <sstream>

//=================================
struct TracerSettings {

	uint max_number_of_interactions_per_photon;
	bool use_multithread_when_possible;
	uint pseudo_random_number_seed;

	static const TracerSettings default_settings;
	TracerSettings();	
};
#endif // __TracerSettings_H_INCLUDED__ 
