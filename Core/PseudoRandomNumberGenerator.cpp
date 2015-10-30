#include "PseudoRandomNumberGenerator.h"
//------------------------------------------------------------------------------
PseudoRandomNumberGenerator PseudoRandomNumberGenerator::void_generator = 
	PseudoRandomNumberGenerator(0);
//------------------------------------------------------------------------------
PseudoRandomNumberGenerator::PseudoRandomNumberGenerator(
	const unsigned new_Seed
) {
	init_inverse_maximum();
	set_seed(new_Seed);
}
//------------------------------------------------------------------------------
PseudoRandomNumberGenerator::PseudoRandomNumberGenerator() {
	init_inverse_maximum();
	set_seed_now_using_system_clock();
}
//------------------------------------------------------------------------------
void PseudoRandomNumberGenerator::set_seed_now_using_system_clock() {
	Seed = std::chrono::system_clock::now().time_since_epoch().count();
	pRNG_mt19937.seed(Seed);
}
//------------------------------------------------------------------------------
double PseudoRandomNumberGenerator::uniform() {
	return double(pRNG_mt19937())*inv_max;
}
//------------------------------------------------------------------------------
unsigned PseudoRandomNumberGenerator::seed()const{
	return Seed;
}
//------------------------------------------------------------------------------
void PseudoRandomNumberGenerator::set_seed(const unsigned new_Seed) {
	Seed = new_Seed;
	pRNG_mt19937.seed(Seed);
}
//------------------------------------------------------------------------------
void PseudoRandomNumberGenerator::init_inverse_maximum() {
	inv_max = 1.0/double(pRNG_mt19937.max());
}
//------------------------------------------------------------------------------