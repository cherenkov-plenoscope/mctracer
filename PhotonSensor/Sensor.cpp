// Copyright 2014 Sebastian A. Mueller
#include "PhotonSensor/Sensor.h"
#include <math.h>
#include <sstream>
using std::vector;
using std::string;

namespace PhotonSensor {

Sensor::Sensor(unsigned int _id, const Frame* _frame):
    id(_id),
    frame(_frame) {}

void Sensor::assign_photon(const Photon* photon) {
    photon_arrival_history.emplace_back(  // PhotonArrival
        // id
        photon->get_simulation_truth_id(),
        // wavelength
        photon->get_wavelength(),
        // arrival_time
        photon->get_time_of_flight(),
        // x
        photon->get_final_intersection().
            position_in_object_frame().x,
        // y
        photon->get_final_intersection().
            position_in_object_frame().y,
        // tx
        -1.0*photon->
            get_final_intersection_incident_vector_in_object_frame().x,
        // ty
        -1.0*photon->
            get_final_intersection_incident_vector_in_object_frame().y);
}

}  // namespace PhotonSensor
