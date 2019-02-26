// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_SENSORS_H_
#define PHOTONSENSOR_SENSORS_H_

#include <vector>
#include <stdexcept>
#include "merlict/sensor/Sensor.h"
#include "merlict/Photon.h"

namespace relleums {
namespace sensor {

std::vector<Sensor*> sort_by_frame(const std::vector<Sensor*>* by_occurence);

void assign_photon_to_sensors(
    const Photon* photon,
    std::vector<Sensor*>* sensors_by_frame);

void assign_photons_to_sensors(
    const std::vector<Photon>* photons,
    std::vector<Sensor*>* sensors_by_frame);


class Sensors {
 public:
    std::vector<Sensor*> by_occurence;
    std::vector<Sensor*> by_frame;
    Sensors();
    explicit Sensors(const std::vector<Sensor*> &sensors);
    void init(const std::vector<Sensor*> &sensors);
    unsigned int size()const;
    Sensor* at(const unsigned int pos);
    Sensor* at_frame(const Frame* frame);
    void assign_photon(const Photon* photon);
    void assign_photons(const std::vector<Photon> *photons);
    void clear_history();

 private:
    void assert_no_two_sensors_have_same_frame()const;

 public:
    class NoSuchFrame : public std::out_of_range {
        using out_of_range::out_of_range;
    };

    class DuplicateFrame : public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };
};

}  // namespace sensor
}  // namespace relleums

#endif  // PHOTONSENSOR_SENSORS_H_
