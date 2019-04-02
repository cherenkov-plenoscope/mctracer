// Copyright 2014 Sebastian A. Mueller
#include "merlict/sensor/PhotonArrival.h"

namespace merlict {
namespace sensor {

PhotonArrival::PhotonArrival() {}

PhotonArrival::PhotonArrival(
    int32_t id,
    double w,
    double t,
    double x,
    double y,
    double tx,
    double ty
):
    simulation_truth_id(id),
    wavelength(w),
    arrival_time(t),
    x_intersect(x),
    y_intersect(y),
    theta_x(tx),
    theta_y(ty)
{}

}  // namespace sensor
}  // namespace merlict
