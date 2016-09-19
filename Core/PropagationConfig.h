//=================================
// include guard
#ifndef __PropagationConfig_H_INCLUDED__
#define __PropagationConfig_H_INCLUDED__

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
struct PropagationConfig {

	uint max_number_of_interactions_per_photon;
	bool use_multithread_when_possible;
	uint pseudo_random_number_seed;

	static const PropagationConfig default_config;
	PropagationConfig();	
};
#endif // __PropagationConfig_H_INCLUDED__ 
