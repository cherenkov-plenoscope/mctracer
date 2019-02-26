// Copyright 2014 Sebastian A. Mueller
#include "merlict/random/Generator.h"
#include <math.h>

namespace relleums {
namespace random {

uint64_t Generator::seed()const {
    return seed_;
}

void Generator::set_seed(const uint64_t seed) {
    this->seed_ = seed;
}

Vec3 Generator::get_point_on_xy_disc_within_radius_slow(const double max_r) {
    const double r = sqrt(uniform())*max_r;
    const double phi = uniform()*2.0*M_PI;
    return Vec3(r*cos(phi), r*sin(phi), 0.0);
}

Vec3 Generator::get_point_on_xy_disc_within_radius(const double r) {
    // using rejection sampling
    // - no constant runtime but fast
    const double r_sq = r*r;
    double x, y;

    do {
        x = r*(uniform()*2.0 - 1.0);
        y = r*(uniform()*2.0 - 1.0);
    }while(x*x + y*y > r_sq);

    return Vec3(x, y, 0.0);
}

double Generator::expovariate(const double rate) {
    // sampling from a poisson distribution
    return -log(uniform())/rate;
}

}  // namespace random
}  // namespace relleums
