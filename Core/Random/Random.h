//==============================================================================
// include guard
#ifndef __RANDOM_H_INCLUDED__
#define __RANDOM_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Generator.h"
#include "Mt19937.h"
#include "FakeConstant.h"
#include "SamplesFromDistribution.h"
//==============================================================================

// the Pseudo random generators
namespace Random {

	static const unsigned int ZERO_SEED = 0;
	static Mt19937 void_generator = Mt19937(ZERO_SEED);
}
#endif // __RANDOM_H_INCLUDED__