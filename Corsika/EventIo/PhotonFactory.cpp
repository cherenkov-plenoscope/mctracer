// Copyright 2014 Sebastian A. Mueller
#include "Corsika/EventIo/PhotonFactory.h"
#include "Core/PhysicalConstants.h"
#include <sstream>
using std::stringstream;
using std::vector;
using std::array;

namespace EventIo {

PhotonFactory::PhotonFactory(
    const array<float, 8> &_corsika_photon,
    const unsigned int _id,
    Random::Generator *prng
):
    id(_id),
    corsika_photon(_corsika_photon) {
    assert_photon_weight_is_between_zero_and_one();
    check_once_if_passed_atmosphere(prng);
}

void PhotonFactory::check_once_if_passed_atmosphere(Random::Generator *prng) {
    _passed_atmosphere = prng->uniform() <= photon_survival_probability();
}

bool PhotonFactory::passed_atmosphere()const {
    return _passed_atmosphere;
}

Photon PhotonFactory::get_photon() {
    Vec3 causal_dir = causal_get_direction();

    Ray ray_running_upwards_from_ground_to_pos_of_production(
        intersection_with_xy_floor_plane(),
        causal_dir*(-1));

    Vec3 causal_support =
        ray_running_upwards_from_ground_to_pos_of_production.
        position_at(ray_parameter_for_production_point());

    Photon cherenkov_photon(causal_support, causal_dir, wavelength());
    cherenkov_photon.set_simulation_truth_id(id);
    return cherenkov_photon;
}

Vec3 PhotonFactory::intersection_with_xy_floor_plane()const {
    return Vec3(x_pos_on_xy_plane(), y_pos_on_xy_plane(), 0.0);
}

double PhotonFactory::production_distance_offset()const {
    // an arbitrary offset distance for the photons to travel until they
    // reach the ground. If set to zero 0.0, the distance for a mctracer photon
    // to travel is only defined by the relative_arrival_time_on_ground().
    // Ensure this offset is at least as big as your detector system on ground.
    return 1e3;
}

double PhotonFactory::ray_parameter_for_production_point()const {
    return relative_arrival_time_on_ground()*
        PhysicalConstants::VACUUM_SPPED_OF_LIGHT + production_distance_offset();
}

double PhotonFactory::x_pos_on_xy_plane()const {
    return corsika_photon[0]*1e-2;
}

double PhotonFactory::y_pos_on_xy_plane()const {
    return corsika_photon[1]*1e-2;
}

Vec3 PhotonFactory::causal_get_direction()const {
    // KIT-CORSIKA coordinate-system
    //
    //                   /\ z-axis                                            //
    //                   |                                                    //
    //                   |\ p                                                 //
    //                   | \ a                                                //
    //                   |  \ r                                               //
    //                   |   \ t                                              //
    //                   |    \ i                                             //
    //                   |     \ c                                            //
    //                   |      \ l                                           //
    //                   |       \ e                                          //
    //                   |        \                                           //
    //                   |  theta  \ m                                        //
    //                   |       ___\ o                                       //
    //                   |___----    \ m      ___                             //
    //                   |            \ e       /| y-axis (west)              //
    //                   |             \ n    /                               //
    //                   |              \ t /                                 //
    //                   |               \/u                                  //
    //                   |              / \ m                                 //
    //                   |            /    \                                  //
    //                   |          /       \                                 //
    //                   |        /__________\                                //
    //                   |      /      ___---/                                //
    //                   |    /   __---    /                                  //
    //                   |  /__--- phi \ /                                    //
    //   ________________|/--__________/______\ x-axis (north)                //
    //                  /|                    /                               //
    //                /  |                                                    //
    //              /    |                                                    //
    //            /                                                           //
    //                                                                        //
    //                                                                        //
    //    Extensive Air Shower Simulation with CORSIKA, Figure 1, page 114
    //    (Version 7.6400 from December 27, 2017)
    //
    //    Direction-cosines:
    //
    //    u = sin(theta) * cos(phi)
    //    v = sin(theta) * sin(phi)
    //
    //    The zenith-angle theta opens relative to the negative z-axis.
    //
    //    It is the momentum of the Cherenkov-photon, which is pointing
    //    down towards the observation-plane.
    //
    const double u = corsika_photon[2];
    const double v = corsika_photon[3];
    const double z = sqrt(1.0 -u*u -v*v);
    return Vec3(u, v, -z);
}

double PhotonFactory::relative_arrival_time_on_ground()const {
    return corsika_photon[4]*1e-9;
}

double PhotonFactory::production_height()const {
    return corsika_photon[5]*1e-2;
}

float PhotonFactory::photon_survival_probability()const {
    return corsika_photon[6];
}

double PhotonFactory::wavelength()const {
    return fabs(corsika_photon[7]*1e-9);
}

void PhotonFactory::assert_photon_weight_is_between_zero_and_one()const {
    if (photon_survival_probability() < 0.0 ||
        photon_survival_probability() > 1.0
    ) {
        stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "Expected photon weight w: 0.0 >= w >= 1.0, but actual ";
        info << "it is: " << photon_survival_probability() << "\n";
        throw BadPhotonWeight(info.str());
    }
}

}  // namespace EventIo
