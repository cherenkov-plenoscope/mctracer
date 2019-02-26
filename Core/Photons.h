// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_PHOTONS_H_
#define CORE_PHOTONS_H_

#include <vector>
#include <string>
#include "merlict/Photon.h"
#include "merlict/PropagationEnvironment.h"

namespace relleums {
namespace Photons {

void propagate_photons_in_scenery_with_settings(
    std::vector<Photon> *photons,
    const Frame *world,
    const PropagationConfig *settings,
    random::Generator* prng);

void propagate_photons(
    std::vector<Photon> *photons,
    const Frame* world,
    const PropagationConfig* settings,
    random::Generator* prng);

void propagate_photons_multi_thread(
    std::vector<Photon> *photons,
    const Frame* world,
    const PropagationConfig* settings,
    random::Generator* prng);

std::vector<Photon> raw_matrix2photons(
    std::vector<std::vector<double>> raw_matrix);

std::vector<std::vector<double>> photons2raw_matrix(
    std::vector<Photon> *photons);

std::vector<double> photon2raw_row(Photon* ph);

Photon raw_row2photon(std::vector<double> &raw_row);

void assert_raw_row_size_matches_photon(std::vector<double> &raw_row);

namespace Source {

std::vector<Photon> parallel_towards_z_from_xy_disc(
    const double disc_radius,
    const unsigned int number_of_photons,
    random::Generator* prng);

std::vector<Photon> point_like_towards_z_opening_angle_num_photons(
    const double opening_angle,
    const unsigned int number_of_photons,
    random::Generator* prng);

}  // namespace Source

void transform_all_photons(const HomTra3 Trafo, std::vector<Photon> *photons);
void transform_all_photons_multi_thread(
    const HomTra3 Trafo,
    std::vector<Photon> *photons
);

}  // namespace Photons
}  // namespace relleums

#endif  // CORE_PHOTONS_H_
