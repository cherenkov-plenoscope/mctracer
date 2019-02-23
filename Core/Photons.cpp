// Copyright 2014 Sebastian A. Mueller
#include "Core/Photons.h"
#include <sstream>
#include <iostream>
#include <future>
#include <thread>
#include "vit_vit_cptl_stl.h"
#include "Core/PhotonAndFrame.h"
using std::string;
using std::stringstream;
using std::vector;

namespace relleums {
namespace Photons {

void propagate_photons_in_scenery_with_settings(
    vector<Photon> *photons,
    const Frame* world,
    const PropagationConfig* settings,
    Random::Generator* prng
) {
    if (settings->use_multithread_when_possible)
        propagate_photons_parallel(photons, world, settings, prng);
    else
        propagate_photons(photons, world, settings, prng);
}

void propagate_photons(
    vector<Photon> *photons,
    const Frame* world,
    const PropagationConfig* settings,
    Random::Generator* prng
) {
    PropagationEnvironment env;
    env.root_frame = world;
    env.config = settings;
    env.prng = prng;

    for (unsigned int i = 0; i < photons->size(); i++)
        PhotonAndFrame::Propagator(&photons->at(i), env);
}

void propagate_one_photon(
    int id,
    const Frame* world,
    const PropagationConfig* settings,
    Random::Generator *prng,
    Photon* photon
) {
    (void)id;
    PropagationEnvironment env;
    env.root_frame = world;
    env.config = settings;
    env.prng = prng;
    PhotonAndFrame::Propagator(photon, env);
}

void propagate_photons_parallel(
    vector<Photon> *photons,
    const Frame* world,
    const PropagationConfig* settings,
    Random::Generator* prng
) {
    uint64_t num_threads = std::thread::hardware_concurrency();
    ctpl::thread_pool pool(num_threads);
    std::vector<std::future<void>> results(photons->size());

    for (uint64_t i = 0; i < photons->size(); ++i) {
        results[i] = pool.push(
            propagate_one_photon,
            world,
            settings,
            prng,
            &(photons->at(i)));
    }

    for (uint64_t i = 0; i < photons->size(); i ++) {
        results[i].get();
    }
}

// In Out to raw matrix/table -> AsciiIO can read/write this to text files

vector<Photon> raw_matrix2photons(vector<vector<double>> raw_matrix) {
    vector<Photon> photons;
    for (vector<double> raw_row : raw_matrix)
        photons.push_back(raw_row2photon(raw_row));
    return photons;
}

vector<vector<double>> photons2raw_matrix(vector<Photon> *photons) {
    vector<vector<double>> raw_matrix;
    for (Photon &ph : *photons)
        raw_matrix.push_back(photon2raw_row(&ph));
    return raw_matrix;
}

vector<double> photon2raw_row(Photon* ph) {
    vector<double> raw_row;
    raw_row.reserve(8);

    raw_row.push_back(static_cast<double>(ph->get_simulation_truth_id()));

    raw_row.push_back(ph->support().x);
    raw_row.push_back(ph->support().y);
    raw_row.push_back(ph->support().z);

    raw_row.push_back(ph->direction().x);
    raw_row.push_back(ph->direction().y);
    raw_row.push_back(ph->direction().z);

    raw_row.push_back(ph->get_wavelength());

    return raw_row;
}

Photon raw_row2photon(vector<double> &raw_row) {
    assert_raw_row_size_matches_photon(raw_row);
    const double id = raw_row[0];
    const Vec3 support(raw_row[1], raw_row[2], raw_row[3]);
    const Vec3 direction(raw_row[4], raw_row[5], raw_row[6]);
    const double wavelength = raw_row[7];

    Photon ph = Photon(support, direction, wavelength);
    ph.set_simulation_truth_id(id);
    return ph;
}

void assert_raw_row_size_matches_photon(vector<double> &raw_row) {
    if (raw_row.size() != 8) {
        stringstream out;
        out << "PhotonBunch, raw row of doubles to photon.\n";
        out << "Expected row to have exactly 8 columns, but actual it has ";
        out << raw_row.size() << " columns.\n";
        throw std::invalid_argument(out.str());
    }
}

namespace Source {

vector<Photon> point_like_towards_z_opening_angle_num_photons(
    const double opening_angle,
    const unsigned int number_of_photons
) {
    vector<Photon> photons;
    photons.reserve(number_of_photons);
    const Vec3 support = VEC3_ORIGIN;

    Random::Mt19937 prng(0);
    Random::ZenithDistancePicker zenith_picker(0.0, opening_angle);
    Random::UniformPicker azimuth_picker(0.0, 2*M_PI);
    for (unsigned int i = 0; i < number_of_photons; i++) {
        Vec3 direction = Random::draw_point_on_sphere(
            &prng,
            zenith_picker,
            azimuth_picker);
        Photon ph = Photon(support, direction, 433e-9);
        ph.set_simulation_truth_id(i);
        photons.push_back(ph);
    }
    return photons;
}

vector<Photon> parallel_towards_z_from_xy_disc(
    const double disc_radius,
    const unsigned int number_of_photons
) {
    vector<Photon> photons;
    photons.reserve(number_of_photons);
    const Vec3 direction = VEC3_UNIT_Z;

    Random::Mt19937 prng(0);

    for (unsigned int i = 0; i < number_of_photons; i++) {
        Vec3 support = prng.get_point_on_xy_disc_within_radius(
            disc_radius);
        Photon ph(support, direction, 433e-9);
        ph.set_simulation_truth_id(i);
        photons.push_back(ph);
    }
    return photons;
}

}  // namespace Source

// transformations, move and rotate photons

void transform_all_photons(const HomTra3 Trafo, vector<Photon> *photons) {
    for (size_t i = 0; i < photons->size(); i++)
        photons->at(i).transform(&Trafo);
}

}  // namespace Photons
}  // namespace relleums
