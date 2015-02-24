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
#include "CartesianFrame.h"
#include "GlobalSettings.h"
#include "PseudoRandomNumberGenerator.h"

class PropagationEnvironment {
public:
	const CartesianFrame* world_geometry = nullptr;
	const GlobalSettings* propagation_options = nullptr;
	PseudoRandomNumberGenerator* random_engine = nullptr;

	void assert_completeness()const;
private:
	void throw_missing_item(const std::string name_of_missing_item)const;
};
#endif // __PropagationEnvironment_H_INCLUDED__ 
