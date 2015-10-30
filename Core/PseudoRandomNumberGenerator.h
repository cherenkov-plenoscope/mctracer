//==============================================================================
// include guard
#ifndef __PSERANNUMGEN_H_INCLUDED__
#define __PSERANNUMGEN_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include <iostream>
#include <chrono>
#include <random>
//==============================================================================

class PseudoRandomNumberGenerator {
	
	unsigned Seed;
	// mt19937 is a standard mersenne_twister_engine
	std::mt19937_64 pRNG_mt19937;
	double inv_max;
public:
	static const uint zero_seed;
	static PseudoRandomNumberGenerator void_generator;
	PseudoRandomNumberGenerator(const unsigned new_Seed);
	PseudoRandomNumberGenerator();
	void set_seed_now_using_system_clock();
	double uniform();
	unsigned seed()const;
	void set_seed(const unsigned new_Seed);
private:
	void init_inverse_maximum();
};
#endif // __PSERANNUMGEN_H_INCLUDED__