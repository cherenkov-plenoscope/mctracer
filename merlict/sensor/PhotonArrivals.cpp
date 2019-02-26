// Copyright 2014 Sebastian A. Mueller
#include "merlict/sensor/PhotonArrivals.h"
#include <math.h>
#include <sstream>
using std::vector;
using std::string;

namespace merlict {
namespace sensor {

double arrival_time_mean(const vector<PhotonArrival>& arrivals) {
    double t = 0.0;
    for (PhotonArrival ph : arrivals)
        t += ph.arrival_time;
    return t/arrivals.size();
}

double x_mean(const vector<PhotonArrival>& arrivals) {
    double xm = 0.0;
    for (PhotonArrival ph : arrivals)
        xm += ph.x_intersect;
    return xm/arrivals.size();
}

double y_mean(const vector<PhotonArrival>& arrivals) {
    double ym = 0.0;
    for (PhotonArrival ph : arrivals)
        ym += ph.y_intersect;
    return ym/arrivals.size();
}

double x_std_dev(const vector<PhotonArrival>& arrivals) {
    double xm = x_mean(arrivals);
    double sx = 0.0;
    for (PhotonArrival ph : arrivals)
        sx += (ph.x_intersect - xm)*(ph.x_intersect - xm);
    return sqrt(sx/arrivals.size());
}

double y_std_dev(const vector<PhotonArrival>& arrivals) {
    double ym = y_mean(arrivals);
    double sy = 0.0;
    for (PhotonArrival ph : arrivals)
        sy += (ph.y_intersect - ym)*(ph.y_intersect - ym);
    return sqrt(sy/arrivals.size());
}

double point_spread_std_dev(const vector<PhotonArrival>& arrivals) {
    return hypot(x_std_dev(arrivals), y_std_dev(arrivals));
}

vector<vector<double>> history_to_table(
    const vector<PhotonArrival>& arrivals
) {
    vector<vector<double>> output_table;
    for (PhotonArrival ph : arrivals) {
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

string arrival_table_header() {
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

}  // namespace sensor
}  // namespace merlict
