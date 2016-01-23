//==============================================================================
// include guard
#ifndef __PRNG_FakeConstant_INCLUDED__
#define __PRNG_FakeConstant_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Generator.h"
//==============================================================================
namespace Random {

	// fake prng retruning always a specific value
	class FakeConstant : public Generator{
		
		double constant;
	public:

		FakeConstant(const double _constant);
		double uniform();
		double normal(const double mean, const double std_dev);
	};
}
#endif // __PRNG_FakeConstant_INCLUDED__