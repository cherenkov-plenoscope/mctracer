// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_PHOTON_H_
#define CORE_PHOTON_H_

namespace merlict {
namespace PhotonAndFrame {

class Propagator;

} // PhotonAndFrame
} // namespace relleums

#include <string>
#include "merlict/RayForPropagation.h"
#include "FresnelRefractionAndReflection.h"

namespace merlict {

class Photon :public RayForPropagation{
    friend class PhotonAndFrame::Propagator;
 protected:
    double wavelength;

 public:
    Photon(
        const Vec3 support,
        const Vec3 direction,
        const double wavelength);
    double get_wavelength()const;
    std::string str()const;
    double get_time_of_flight()const;

 private:
    double get_time_to_pass_distance_in_refractive_index(
        const double distance_in_medium,
        const double refractive_index_in_medium)const;
    void assert_wavelength_is_positive()const;
};

}  // namespace merlict

#endif  // CORE_PHOTON_H_
