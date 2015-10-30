#include "PseudoRandomNumberGenerator.h"
#include <chrono>
using namespace Random;
//------------------------------------------------------------------------------
Mt19937 void_generator = Mt19937(zero_seed);
//------------------------------------------------------------------------------
unsigned Generator::get_seed()const{
	return seed;
}
//------------------------------------------------------------------------------
void Generator::set_seed_now_using_system_clock() {
	set_seed(
		std::chrono::system_clock::now().time_since_epoch().count()
	);
}
//------------------------------------------------------------------------------
void Generator::set_seed(const unsigned _seed) {
	seed = _seed;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Mt19937::Mt19937(const unsigned _seed) {
	init_inverse_maximum();
	set_seed(_seed);
}
//------------------------------------------------------------------------------
Mt19937::Mt19937() {
	init_inverse_maximum();
	set_seed_now_using_system_clock();
}
//------------------------------------------------------------------------------
double Mt19937::uniform() {
	return double(pRNG_mt19937())*inv_max;
}
//------------------------------------------------------------------------------
void Mt19937::set_seed(const unsigned _seed) {
	seed = _seed;
	pRNG_mt19937.seed(seed);
}
//------------------------------------------------------------------------------
void Mt19937::init_inverse_maximum() {
	inv_max = 1.0/double(pRNG_mt19937.max());
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
FakeConstant::FakeConstant(const double _constant) {
	constant = _constant;
	seed = zero_seed;
}
//------------------------------------------------------------------------------
double FakeConstant::uniform() {
	return constant;
}
//------------------------------------------------------------------------------
