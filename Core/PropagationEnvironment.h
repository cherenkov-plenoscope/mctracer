//=================================
// include guard
#ifndef __PropagationEnvironment_H_INCLUDED__
#define __PropagationEnvironment_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Frame.h"
#include "Core/PropagationConfig.h"
#include "Core/Random/Random.h"

class PropagationEnvironment {
public:

	const Frame* world_geometry = &Frame::void_frame;
	const PropagationConfig* propagation_options = 
		&PropagationConfig::default_config;
	Random::Generator* random_engine =
		&Random::void_generator;
		
	static PropagationEnvironment default_environment;
};
#endif // __PropagationEnvironment_H_INCLUDED__ 
