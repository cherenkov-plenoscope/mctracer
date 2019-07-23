// Copyright 2014 Sebastian A. Mueller
#ifndef CORSIKA_PHOTONFACTORY_H_
#define CORSIKA_PHOTONFACTORY_H_

#include <array>
#include "merlict/merlict.h"

namespace merlict {

struct EventIoPhotonFactory {
    unsigned int id;
    unsigned int num_photons;
    unsigned int num_photons_made;
    std::array<float, 8> corsika_photon;

    class BadPhotonWeight : public std::runtime_error {
        using runtime_error::runtime_error;
    };
    EventIoPhotonFactory(
        const std::array<float, 8>& _corsika_photon,
        const unsigned int id,
        random::Generator *prng);
    bool has_still_photons_to_be_made()const;
    Photon make_photon();
    Vec3 direction_of_motion()const;
    Vec3 intersection_with_xy_floor_plane()const;
    double ray_parameter_for_production_point()const;
    double x_pos_on_xy_plane()const;
    double y_pos_on_xy_plane()const;
    double production_height()const;
    double wavelength()const;
    double production_distance_offset()const;
    double relative_arrival_time_on_ground()const;
    void assert_bunch_weight(const double)const;
};

}  // namespace merlict

#endif  // CORSIKA_PHOTONFACTORY_H_
