// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_SENSOR_H_
#define PHOTONSENSOR_SENSOR_H_

#include <vector>
#include <string>
#include "Core/Photon.h"
#include "PhotonArrival.h"

namespace PhotonSensor {

struct Sensor {
    unsigned int id;
    const Frame* frame;
    std::vector<PhotonArrival> photon_arrival_history;
    Sensor(unsigned int id, const Frame* frame);
    void assign_photon(const Photon* photon);
    double x_mean()const;
    double y_mean()const;
    double x_std_dev()const;
    double y_std_dev()const;
    double point_spread_std_dev()const;
    double arrival_time_mean()const;
    std::vector<std::vector<double>> get_arrival_table()const;
    std::string get_arrival_table_header()const;
    struct FrameSensorByFramePointerCompare {
        bool operator()(const Frame* f, const Sensor* s) {
            return f < s->frame;
        }
    };
};

}  // namespace PhotonSensor

#endif  // PHOTONSENSOR_SENSOR_H_
