// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_CORSIKA_EVENTIO_PHOTONFACTORY_H_
#define MCTRACER_CORSIKA_EVENTIO_PHOTONFACTORY_H_

#include <array>
#include "Core/Photon.h"
#include "Core/Random/Random.h"

namespace EventIo {

class PhotonFactory {
    bool _passed_atmosphere = false;
    unsigned int id;
    std::array<float, 8> corsika_photon;

 public:
    class BadPhotonWeight : public std::runtime_error {
        using runtime_error::runtime_error;
    };
    PhotonFactory(
        const std::array<float, 8>& _corsika_photon,
        const unsigned int id,
        Random::Generator *prng);
    bool passed_atmosphere()const;
    Photon get_photon();
    Vec3 causal_get_direction()const;
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
    void check_once_if_passed_atmosphere(Random::Generator *prng);
    void assert_corsika_photon_has_correct_length()const;
    void assert_photon_weight_is_between_zero_and_one()const;
};

}  // namespace EventIo

#endif  // MCTRACER_CORSIKA_EVENTIO_PHOTONFACTORY_H_
