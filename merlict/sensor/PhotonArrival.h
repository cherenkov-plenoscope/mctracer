// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_PHOTONARRIVAL_H_
#define PHOTONSENSOR_PHOTONARRIVAL_H_

#include <stdint.h>

namespace merlict {
namespace sensor {

struct PhotonArrival {
    int32_t simulation_truth_id;
    double wavelength;
    double arrival_time;
    double x_intersect;
    double y_intersect;
    double theta_x;
    double theta_y;

    PhotonArrival();
    PhotonArrival(
        int32_t id,
        double w,
        double t,
        double x,
        double y,
        double tx,
        double ty);
};

}  // namespace sensor
}  // namespace relleums

#endif  // PHOTONSENSOR_PHOTONARRIVAL_H_
