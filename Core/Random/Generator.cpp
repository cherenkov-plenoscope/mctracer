#include "Generator.h"
#include <chrono>
#include "Random.h"
using namespace Random;
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
double Generator::expovariate(const double rate) {
	// sampling from a poisson distribution
	return -log(uniform())/rate;
}