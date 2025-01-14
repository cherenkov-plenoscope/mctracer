// Copyright 2014 Sebastian A. Mueller
#include "merlict_corsika/PhotonFactory.h"
#include <sstream>


namespace merlict {

EventIoPhotonFactory::EventIoPhotonFactory(
    const std::array<float, 8> &_corsika_photon,
    const unsigned int _id,
    random::Generator *prng
):
    id(_id),
    num_photons(0),
    num_photons_made(0),
    corsika_photon(_corsika_photon) {
    const double bunch_weight = corsika_photon[6];
    assert_bunch_weight(bunch_weight);
    num_photons = static_cast<unsigned int>(floor(bunch_weight));
    double remaining_bunch_weight = fmod(bunch_weight, 1.);
    if (prng->uniform() <= remaining_bunch_weight) {
        num_photons += 1;
    }
}

bool EventIoPhotonFactory::has_still_photons_to_be_made()const {
    return num_photons_made < num_photons;
}

Photon EventIoPhotonFactory::make_photon() {
    Vec3 causal_dir = direction_of_motion();

    Ray ray_running_upwards_from_ground_to_pos_of_production(
        intersection_with_xy_floor_plane(),
        causal_dir*(-1));

    Vec3 causal_support =
        ray_running_upwards_from_ground_to_pos_of_production.
        position_at(ray_parameter_for_production_point());

    Photon cherenkov_photon(causal_support, causal_dir, wavelength());
    cherenkov_photon.simulation_truth_id = id;
    num_photons_made += 1;
    return cherenkov_photon;
}

Vec3 EventIoPhotonFactory::intersection_with_xy_floor_plane()const {
    return Vec3(x_pos_on_xy_plane(), y_pos_on_xy_plane(), 0.0);
}

double EventIoPhotonFactory::production_distance_offset()const {
    // an arbitrary offset distance for the photons to travel until they
    // reach the ground. If set to zero 0.0, the distance for a merlict photon
    // to travel is only defined by the relative_arrival_time_on_ground().
    // Ensure this offset is at least as big as your detector system on ground.
    return 1e3;
}

double EventIoPhotonFactory::ray_parameter_for_production_point()const {
    return relative_arrival_time_on_ground()*
        VACUUM_SPPED_OF_LIGHT + production_distance_offset();
}

double EventIoPhotonFactory::x_pos_on_xy_plane()const {
    return corsika_photon[0]*1e-2;
}

double EventIoPhotonFactory::y_pos_on_xy_plane()const {
    return corsika_photon[1]*1e-2;
}

Vec3 EventIoPhotonFactory::direction_of_motion()const {
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

double EventIoPhotonFactory::relative_arrival_time_on_ground()const {
    return corsika_photon[4]*1e-9;
}

double EventIoPhotonFactory::production_height()const {
    return corsika_photon[5]*1e-2;
}

double EventIoPhotonFactory::wavelength()const {
    return fabs(corsika_photon[7]*1e-9);
}

void EventIoPhotonFactory::assert_bunch_weight(const double bunch_weight)const {
    if (bunch_weight < 0. || bunch_weight > 16.) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "Expected bunch-weight w: 0.0 >= w >= 16.0, but actual ";
        info << "w is: " << bunch_weight << "\n";
        throw BadPhotonWeight(info.str());
    }
}

}  // namespace merlict
