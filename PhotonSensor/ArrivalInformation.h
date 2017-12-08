// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_PHOTONSENSOR_ARRIVALINFORMATION_H_
#define MCTRACER_PHOTONSENSOR_ARRIVALINFORMATION_H_

namespace PhotonSensor {

struct ArrivalInformation {
    int simulation_truth_id;
    double wavelength;
    double arrival_time;
    double x_intersect;
    double y_intersect;
    double theta_x;
    double theta_y;

    ArrivalInformation();
    ArrivalInformation(
        int id,
        double w,
        double t,
        double x,
        double y,
        double tx,
        double ty);
};

}  // namespace PhotonSensor

#endif  // MCTRACER_PHOTONSENSOR_ARRIVALINFORMATION_H_
