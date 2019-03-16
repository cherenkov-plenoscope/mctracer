// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_PHOTONS_H_
#define MERLICT_PHOTONS_H_

#include <vector>
#include <string>
#include "merlict/Photon.h"
#include "merlict/PropagationEnvironment.h"

namespace merlict {

void propagate_photons_in_frame_with_config(
    std::vector<Photon> *photons,
    const Frame *world,
    const PropagationConfig *settings,
    random::Generator* prng);

void propagate_photons_in_frame_with_config_multi_thread(
    std::vector<Photon> *photons,
    const Frame* world,
    const PropagationConfig* settings,
    random::Generator* prng);

void apply_transformation_to_photons(
    const HomTra3 Trafo,
    std::vector<Photon> *photons);

std::vector<Photon> raw_matrix2photons(
    std::vector<std::vector<double>> raw_matrix);

std::vector<std::vector<double>> photons2raw_matrix(
    std::vector<Photon> *photons);

std::vector<double> photon2raw_row(Photon* ph);

Photon raw_row2photon(std::vector<double> &raw_row);

void assert_raw_row_size_matches_photon(std::vector<double> &raw_row);

namespace photon_source {

std::vector<Photon> parallel_towards_z_from_xy_disc(
    const double disc_radius,
    const unsigned int num_photons,
    random::Generator* prng);

std::vector<Photon> point_like_towards_z_opening_angle_num_photons(
    const double opening_angle,
    const unsigned int num_photons,
    random::Generator* prng);

}  // namespace photon_source

}  // namespace merlict

#endif  // MERLICT_PHOTONS_H_
