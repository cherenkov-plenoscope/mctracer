// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_INOUT_H_
#define PHOTONSENSOR_INOUT_H_

#include <fstream>
#include <vector>
#include "sensor/PhotonArrival.h"

namespace relleums {
namespace sensor {

void write_arrival_information_to_file(
    const std::vector<PhotonArrival>* arrival_table,
    std::ofstream *file);

std::vector<PhotonArrival> read_arrival_information_from_file(
    std::ifstream *file,
    const unsigned int number_of_arrivals);

}  // namespace sensor
}  // namespace relleums

#endif  // PHOTONSENSOR_INOUT_H_