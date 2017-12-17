// Copyright 2014 Sebastian A. Mueller
#include "PhotonSensor/PhotonArrival.h"

namespace PhotonSensor {

PhotonArrival::PhotonArrival() {}

PhotonArrival::PhotonArrival(
    int id,
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

}  // namespace PhotonSensor
