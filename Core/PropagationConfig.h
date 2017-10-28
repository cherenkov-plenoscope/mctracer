//=================================
// include guard
#ifndef __PropagationConfig_H_INCLUDED__
#define __PropagationConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Color.h"
#include "Vec3.h"
#include <string>
#include <sstream>

//=================================
struct PropagationConfig {

	uint max_number_of_interactions_per_photon;
	bool use_multithread_when_possible;

	static const PropagationConfig default_config;
	PropagationConfig();	
};
#endif // __PropagationConfig_H_INCLUDED__ 
