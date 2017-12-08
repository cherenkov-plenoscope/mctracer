// Copyright 2014 Sebastian A. Mueller
#include "PhotonSensor/inout.h"
#include <array>
using std::array;
using std::vector;

namespace PhotonSensor {

void write_arrival_information_to_file(
    const vector<ArrivalInformation>* arrival_table,
    std::ofstream *file
) {
    for (unsigned int i = 0; i < arrival_table->size(); i++) {
        // convert to float32
        array<float, 7> out;
        out[0] = (float)arrival_table->at(i).simulation_truth_id;
        out[1] = (float)arrival_table->at(i).wavelength;
        out[2] = (float)arrival_table->at(i).arrival_time;
        out[3] = (float)arrival_table->at(i).x_intersect;
        out[4] = (float)arrival_table->at(i).y_intersect;
        out[5] = (float)arrival_table->at(i).theta_x;
        out[6] = (float)arrival_table->at(i).theta_y;
        file->write((char*)out.data(), out.size()*sizeof(float));
    }
}

vector<ArrivalInformation> read_arrival_information_from_file(
    std::ifstream *file,
    const unsigned int number_of_arrivals
) {
    vector<ArrivalInformation> arrivals;
    arrivals.reserve(number_of_arrivals);

    for (unsigned int i = 0; i < number_of_arrivals; i++) {
        array<float, 7> block;
        file->read((char*)block.data(), block.size()*sizeof(float));

        ArrivalInformation arrival;

        arrivals.emplace_back(
            (int)block[0],      // simulation_truth_id
            (double)block[1],   // wavelength
            (double)block[2],   // arrival_time
            (double)block[3],   // x_intersect
            (double)block[4],   // y_intersect
            (double)block[5],   // theta_x
            (double)block[6]);  // theta_y
    }
    return arrivals;
}

}  // namespace PhotonSensor
