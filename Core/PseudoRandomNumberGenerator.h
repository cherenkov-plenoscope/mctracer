//==============================================================================
// include guard
#ifndef __PSERANNUMGEN_H_INCLUDED__
#define __PSERANNUMGEN_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies
#include <iostream>
#include <random>
//==============================================================================

// the Pseudo random generators
namespace Random {

	class Generator {
	protected:

		unsigned seed;
	public:

		uint get_seed()const;
		virtual double uniform() = 0;
		virtual void set_seed(const uint _seed);
		void set_seed_now_using_system_clock();
	};

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

	// fake prng retruning alway a specific value
	class FakeConstant : public Generator{
		
		double constant;
	public:

		FakeConstant(const double _constant);
		double uniform();
	};	

	static const uint zero_seed = 0;
	static Mt19937 void_generator;	
}
#endif // __PSERANNUMGEN_H_INCLUDED__