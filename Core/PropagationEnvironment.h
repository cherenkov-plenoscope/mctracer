//=================================
// include guard
#ifndef __PropagationEnvironment_H_INCLUDED__
#define __PropagationEnvironment_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include "TracerException.h"
#include "Core/Frame.h"
#include "Core/TracerSettings.h"
#include "Core/PseudoRandomNumberGenerator.h"

class PropagationEnvironment {
public:
	const Frame* world_geometry = nullptr;
	const TracerSettings* propagation_options = nullptr;
	PseudoRandomNumberGenerator* random_engine = nullptr;

	void assert_completeness()const;
private:
	void throw_missing_item(const std::string name_of_missing_item)const;
};
#endif // __PropagationEnvironment_H_INCLUDED__ 
