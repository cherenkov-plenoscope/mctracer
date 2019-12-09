// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_SENSOR_H_
#define PHOTONSENSOR_SENSOR_H_

#include <vector>
#include <string>
#include "merlict/Photon.h"
#include "PhotonArrival.h"

namespace merlict {
namespace sensor {

struct Sensor {
    unsigned int id;
    std::shared_ptr<const Frame> frame;
    std::vector<PhotonArrival> photon_arrival_history;
    Sensor(unsigned int id, std::shared_ptr<const Frame> frame);
    void assign_photon(const Photon* photon);
    struct FrameSensorByFramePointerCompare {
        bool operator()(std::shared_ptr<const Frame> f, const Sensor* s) {
            return f < s->frame;
        }
    };
};

}  // namespace sensor
}  // namespace merlict

#endif  // PHOTONSENSOR_SENSOR_H_
