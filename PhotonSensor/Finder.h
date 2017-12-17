// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_PHOTONSENSOR_FINDER_H_
#define MCTRACER_PHOTONSENSOR_FINDER_H_

#include <vector>
#include "PhotonSensor/PhotonSensor.h"

namespace PhotonSensor {

struct FindSensorByFrame {
    bool is_absorbed_by_known_sensor;
    PhotonSensor::Sensor* final_sensor;

    FindSensorByFrame(
        const Frame* final_frame,
        const std::vector<PhotonSensor::Sensor*> *sensors);
};

}  // namespace PhotonSensor

#endif  // MCTRACER_PHOTONSENSOR_FINDER_H_
