// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_FINDSENSORBYFRAME_H_
#define PHOTONSENSOR_FINDSENSORBYFRAME_H_

#include <vector>
#include "PhotonSensor/Sensor.h"

namespace PhotonSensor {

struct FindSensorByFrame {
    bool is_absorbed_by_known_sensor;
    PhotonSensor::Sensor* final_sensor;

    FindSensorByFrame(
        const Frame* final_frame,
        const std::vector<PhotonSensor::Sensor*>* sensors_by_frame);
};

}  // namespace PhotonSensor

#endif  // PHOTONSENSOR_FINDSENSORBYFRAME_H_
