// Copyright 2014 Sebastian A. Mueller
#include "merlict/sensor/Sensor.h"
#include <math.h>
#include <sstream>


namespace merlict {
namespace sensor {

Sensor::Sensor(unsigned int _id, std::shared_ptr<const Frame> _frame):
    id(_id),
    frame(_frame) {}

void Sensor::assign_photon(const Photon* photon) {
    photon_arrival_history.emplace_back(  // PhotonArrival
        // id
        photon->simulation_truth_id,
        // wavelength
        photon->wavelength,
        // arrival_time
        photon->time_of_flight(),
        // x
        photon->final_intersection().position_in_object_frame().x,
        // y
        photon->final_intersection().position_in_object_frame().y,
        // tx
        -1.0*photon->final_intersection_incident_direction_wrt_frame().x,
        // ty
        -1.0*photon->final_intersection_incident_direction_wrt_frame().y);
}

}  // namespace sensor
}  // namespace merlict
