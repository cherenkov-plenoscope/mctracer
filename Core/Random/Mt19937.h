//==============================================================================
// include guard
#ifndef __PRNG_Mt19937_INCLUDED__
#define __PRNG_Mt19937_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include "Generator.h"
#include <random>
//==============================================================================
namespace Random {

	// mt19937 is a standard mersenne_twister_engine
	class Mt19937 : public Generator{
		
		std::mt19937_64 pRNG_mt19937;
		double inv_max;
	public:

		Mt19937(const uint32_t seed);
		Mt19937();
		double uniform();
		double normal(const double mean, const double std_dev);
		void set_seed(const uint32_t seed);
	private:

		void init_inverse_maximum();
	};
}
#endif // __PRNG_Mt19937_INCLUDED__