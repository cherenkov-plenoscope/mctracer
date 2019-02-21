// Copyright 2014 Sebastian A. Mueller
#ifndef PHOTONSENSOR_PHOTONARRIVALS_H_
#define PHOTONSENSOR_PHOTONARRIVALS_H_

#include <vector>
#include <string>
#include "PhotonSensor/PhotonArrival.h"

namespace relleums {
namespace PhotonSensor {

double x_mean(const std::vector<PhotonArrival>& arrivals);
double y_mean(const std::vector<PhotonArrival>& arrivals);
double x_std_dev(const std::vector<PhotonArrival>& arrivals);
double y_std_dev(const std::vector<PhotonArrival>& arrivals);
double point_spread_std_dev(const std::vector<PhotonArrival>& arrivals);
double arrival_time_mean(const std::vector<PhotonArrival>& arrivals);
std::vector<std::vector<double>> history_to_table(
    const std::vector<PhotonArrival>& arrivals);
std::string arrival_table_header();

}  // namespace PhotonSensor
}  // namespace relleums

#endif  // PHOTONSENSOR_PHOTONARRIVALS_H_
