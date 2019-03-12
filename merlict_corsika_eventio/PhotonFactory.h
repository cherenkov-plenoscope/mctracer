// Copyright 2014 Sebastian A. Mueller
#ifndef CORSIKA_PHOTONFACTORY_H_
#define CORSIKA_PHOTONFACTORY_H_

#include <array>
#include "merlict/merlict.h"

namespace merlict {

class EventIoPhotonFactory {
    bool _passed_atmosphere = false;
    unsigned int id;
    std::array<float, 8> corsika_photon;

 public:
    class BadPhotonWeight : public std::runtime_error {
        using runtime_error::runtime_error;
    };
    EventIoPhotonFactory(
        const std::array<float, 8>& _corsika_photon,
        const unsigned int id,
        random::Generator *prng);
    bool passed_atmosphere()const;
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
    float photon_survival_probability()const;

 private:
    void check_once_if_passed_atmosphere(random::Generator *prng);
    void assert_corsika_photon_has_correct_length()const;
    void assert_photon_weight_is_between_zero_and_one()const;
};

}  // namespace merlict

#endif  // CORSIKA_PHOTONFACTORY_H_
