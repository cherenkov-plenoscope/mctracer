// Copyright 2014 Sebastian A. Mueller
#include "PhotonSensor/Sensor.h"
#include <math.h>
#include <sstream>
using std::vector;
using std::string;

namespace PhotonSensor {

Sensor::Sensor(unsigned int _id, const Frame* _frame) {
    id = _id;
    frame = _frame;
}

unsigned int Sensor::get_id()const {
    return id;
}

void Sensor::assign_photon_to_this_sensor(const Photon* photon) {
    photon_arrival_history.emplace_back(  // ArrivalInformation
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

double Sensor::arrival_time_mean()const {
    double t = 0.0;
    for (ArrivalInformation ph : photon_arrival_history)
        t += ph.arrival_time;
    return t/photon_arrival_history.size();
}

double Sensor::x_mean()const {
    double xm = 0.0;
    for (ArrivalInformation ph : photon_arrival_history)
        xm += ph.x_intersect;
    return xm/photon_arrival_history.size();
}

double Sensor::y_mean()const {
    double ym = 0.0;
    for (ArrivalInformation ph : photon_arrival_history)
        ym += ph.y_intersect;
    return ym/photon_arrival_history.size();
}

double Sensor::x_std_dev()const {
    double xm = x_mean();
    double sx = 0.0;
    for (ArrivalInformation ph : photon_arrival_history)
        sx += (ph.x_intersect - xm)*(ph.x_intersect - xm);
    return sqrt(sx/photon_arrival_history.size());
}

double Sensor::y_std_dev()const {
    double ym = y_mean();
    double sy = 0.0;
    for (ArrivalInformation ph : photon_arrival_history)
        sy += (ph.y_intersect - ym)*(ph.y_intersect - ym);
    return sqrt(sy/photon_arrival_history.size());
}

double Sensor::point_spread_std_dev()const {
    return hypot(x_std_dev(), y_std_dev());
}

vector<vector<double>> Sensor::get_arrival_table()const {
    vector<vector<double>> output_table;
    for (ArrivalInformation ph : photon_arrival_history) {
        vector<double> output_row;
        output_row.push_back(ph.x_intersect);
        output_row.push_back(ph.y_intersect);
        output_row.push_back(ph.theta_x);
        output_row.push_back(ph.theta_y);
        output_row.push_back(ph.wavelength);
        output_row.push_back(ph.arrival_time);
        output_row.push_back(ph.simulation_truth_id);
        output_table.push_back(output_row);
    }
    return output_table;
}

string Sensor::get_arrival_table_header()const {
    std::stringstream header;
    header << "x intersection [m]\n";
    header << "y intersection [m]\n";
    header << "inverse_incident cos_x [1]\n";
    header << "inverse_incident cos_y [1] inverse_incident = ";
    header << "(cos_x, cos_y, sqrt(1 - cos_x^2 - cos_y^2))^T\n";
    header << "wavelength [m]\n";
    header << "relative arrival time t [s]\n";
    header << "photon id [1]\n";
    header << "x\ty\tcos_x\tcos_y\twavelength\tt\tid\n";
    return header.str();
}

}  // namespace PhotonSensor
