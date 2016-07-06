//=================================
// include guard
#ifndef __PropagationEnvironment_H_INCLUDED__
#define __PropagationEnvironment_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Frame.h"
#include "Core/TracerSettings.h"
#include "Core/Random/Random.h"

class PropagationEnvironment {
public:

	const Frame* world_geometry = &Frame::void_frame;
	const TracerSettings* propagation_options = 
		&TracerSettings::default_settings;
	Random::Generator* random_engine =
		&Random::void_generator;
		
	static PropagationEnvironment default_environment;
};
#endif // __PropagationEnvironment_H_INCLUDED__ 
