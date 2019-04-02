// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_INOUT_H_
#define PHOTONSENSOR_INOUT_H_

#include <fstream>
#include <vector>
#include "merlict/sensor/PhotonArrival.h"

namespace merlict {
namespace sensor {

void write_arrival_information_to_file(
    const std::vector<PhotonArrival>* arrival_table,
    std::ofstream *file);

std::vector<PhotonArrival> read_arrival_information_from_file(
    std::ifstream *file,
    const unsigned int num_arrivals);

}  // namespace sensor
}  // namespace merlict

#endif  // PHOTONSENSOR_INOUT_H_
