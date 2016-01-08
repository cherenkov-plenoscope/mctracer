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
}// Random