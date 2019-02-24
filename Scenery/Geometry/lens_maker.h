// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_LENSMAKER_LENSMAKER_H_
#define SCENERY_GEOMETRY_LENSMAKER_LENSMAKER_H_

namespace relleums {
namespace lens_maker {

struct Config {
    double focal_length;
    double aperture_radius;
    double refractive_index;
    Config();
};

double get_curvature_radius(const Config &cfg);

double get_lens_thickness_for_R_r(const double R, const double r);

void abort_if_too_many_iterations(
    const Config &cfg,
    const unsigned int iteration_conter,
    const double R);

double get_expected_focal_length_for_R_t_n(
    const double R,
    const double t,
    const double n);

}  // namespace lens_maker
}  // namespace relleums

#endif  // SCENERY_GEOMETRY_LENSMAKER_LENSMAKER_H_
