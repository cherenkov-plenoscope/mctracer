// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_FINDSENSORBYFRAME_H_
#define PHOTONSENSOR_FINDSENSORBYFRAME_H_

#include <vector>
#include "merlict/sensor/Sensor.h"

namespace merlict {
namespace sensor {

struct FindSensorByFrame {
    bool is_absorbed_by_known_sensor;
    sensor::Sensor* final_sensor;

    FindSensorByFrame(
        const Frame* final_frame,
        const std::vector<sensor::Sensor*>* sensors_by_frame);
};

}  // namespace sensor
}  // namespace relleums

#endif  // PHOTONSENSOR_FINDSENSORBYFRAME_H_
