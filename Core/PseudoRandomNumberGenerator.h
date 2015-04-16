//==============================================================================
// include guard
#ifndef __PSERANNUMGEN_H_INCLUDED__
#define __PSERANNUMGEN_H_INCLUDED__

//==============================================================================
// forward declared dependencies

//==============================================================================
// included dependencies

//==============================================================================

class PseudoRandomNumberGenerator{
	
	unsigned Seed;
	// mt19937 is a standard mersenne_twister_engine
	std::mt19937 pRNG_mt19937;
public:
	//--------------------------------------------------------------------------
	PseudoRandomNumberGenerator(){
		set_seed_now_using_system_clock();
	};
	//--------------------------------------------------------------------------
	void set_seed_now_using_system_clock() {
		Seed = std::chrono::system_clock::now().time_since_epoch().count();
		pRNG_mt19937.seed(Seed);
	};
	//--------------------------------------------------------------------------
	double uniform() {
		return double(pRNG_mt19937())/double(pRNG_mt19937.max());
	};
	//--------------------------------------------------------------------------
	unsigned seed()const{
		return Seed;
	};
	//--------------------------------------------------------------------------
	void set_seed(unsigned new_Seed){
		pRNG_mt19937.seed(new_Seed);
	};
	//--------------------------------------------------------------------------
};
#endif // __PSERANNUMGEN_H_INCLUDED__