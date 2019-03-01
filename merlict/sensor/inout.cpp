// Copyright 2014 Sebastian A. Mueller
#include "merlict/sensor/inout.h"
#include <array>


namespace merlict {
namespace sensor {

void write_arrival_information_to_file(
    const std::vector<PhotonArrival>* arrival_table,
    std::ofstream *file
) {
    for (unsigned int i = 0; i < arrival_table->size(); i++) {
        std::array<float, 7> out;
        out[0] = static_cast<float>(arrival_table->at(i).simulation_truth_id);
        out[1] = static_cast<float>(arrival_table->at(i).wavelength);
        out[2] = static_cast<float>(arrival_table->at(i).arrival_time);
        out[3] = static_cast<float>(arrival_table->at(i).x_intersect);
        out[4] = static_cast<float>(arrival_table->at(i).y_intersect);
        out[5] = static_cast<float>(arrival_table->at(i).theta_x);
        out[6] = static_cast<float>(arrival_table->at(i).theta_y);
        file->write(
            reinterpret_cast<const char*>(out.data()),
            out.size()*sizeof(float));
    }
}

std::vector<PhotonArrival> read_arrival_information_from_file(
    std::ifstream *file,
    const unsigned int number_of_arrivals
) {
    std::vector<PhotonArrival> arrivals;
    arrivals.reserve(number_of_arrivals);

    for (unsigned int i = 0; i < number_of_arrivals; i++) {
        std::array<float, 7> block;
        file->read(
            reinterpret_cast<char*>(block.data()),
            block.size()*sizeof(float));
        PhotonArrival arrival;
        arrivals.emplace_back(
            static_cast<int>(block[0]),      // simulation_truth_id
            static_cast<double>(block[1]),   // wavelength
            static_cast<double>(block[2]),   // arrival_time
            static_cast<double>(block[3]),   // x_intersect
            static_cast<double>(block[4]),   // y_intersect
            static_cast<double>(block[5]),   // theta_x
            static_cast<double>(block[6]));  // theta_y
    }
    return arrivals;
}

}  // namespace sensor
}  // namespace merlict
