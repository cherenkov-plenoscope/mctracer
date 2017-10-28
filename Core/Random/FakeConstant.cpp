#include "FakeConstant.h"
#include "Random.h"
namespace Random {
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
double FakeConstant::normal(const double mean, const double std_dev) {
	(void)mean;
	(void)std_dev;
	return constant;
}
//------------------------------------------------------------------------------
}// Random