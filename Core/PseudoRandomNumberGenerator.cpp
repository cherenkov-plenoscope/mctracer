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
Vector3D Generator::get_point_on_xy_disc_within_radius(
	const double disc_radius
) {
	const double r = sqrt(uniform())*disc_radius;
	const double phi = uniform()*2.0*M_PI;
	return Vector3D(r*cos(phi), r*sin(phi),	0.0);
}
//------------------------------------------------------------------------------
Vector3D Generator::get_point_on_unitsphere_within_polar_distance(
	const double max_dist
) {
	const double The = max_dist*sqrt(uniform());
	const double Phi = uniform()*2.0*M_PI;

	return Vector3D(
		sin(The)*cos(Phi),
		sin(The)*sin(Phi),
		cos(The)
	);
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
