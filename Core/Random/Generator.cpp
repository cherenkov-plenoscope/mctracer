// Copyright 2014 Sebastian A. Mueller
#include "Core/Random/Generator.h"
#include <math.h>
#include <iostream>


namespace Random {

uint32_t Generator::get_seed()const {
    return seed;
}

void Generator::set_seed(const uint32_t seed) {
    this->seed = seed;
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

Vec3 Generator::get_point_on_unitsphere_within_polar_distance(
    const double max_dist
) {
    const double The = max_dist*sqrt(uniform());
    const double Phi = uniform()*2.0*M_PI;

    return Vec3(
        sin(The)*cos(Phi),
        sin(The)*sin(Phi),
        cos(The));
}

double Generator::expovariate(const double rate) {
    // sampling from a poisson distribution
    return -log(uniform())/rate;
}

}  // namespace Random
