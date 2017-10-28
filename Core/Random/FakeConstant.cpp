#include "FakeConstant.h"
#include "Random.h"

namespace Random {
//------------------------------------------------------------------------------
FakeConstant::FakeConstant(const double _constant) {
	constant = _constant;
	seed = ZERO_SEED;
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