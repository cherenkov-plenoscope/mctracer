// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_PHOTONSENSOR_SENSOR_H_
#define MCTRACER_PHOTONSENSOR_SENSOR_H_

#include <vector>
#include <string>
#include "Core/Photon.h"
#include "ArrivalInformation.h"

namespace PhotonSensor {

struct Sensor {
    const Frame* frame;
    std::vector<ArrivalInformation> photon_arrival_history;
    unsigned int id;
    Sensor(unsigned int _id, const Frame* frame);
    unsigned int get_id()const;
    void assign_photon_to_this_sensor(const Photon* photon);
    void clear_history();
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

#endif  // MCTRACER_PHOTONSENSOR_SENSOR_H_
