//==============================================================================
// include guard
#ifndef __PRNG_Mt19937_INCLUDED__
#define __PRNG_Mt19937_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Generator.h"
//==============================================================================
namespace Random {

	// mt19937 is a standard mersenne_twister_engine
	class Mt19937 : public Generator{
		
		std::mt19937_64 pRNG_mt19937;
		double inv_max;
	public:

		Mt19937(const uint _seed);
		Mt19937();
		double uniform();
		void set_seed(const uint _seed);
	private:

		void init_inverse_maximum();
	};
}
#endif // __PRNG_Mt19937_INCLUDED__