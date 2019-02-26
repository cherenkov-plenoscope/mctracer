// Copyright 2014 Sebastian A. Mueller
#include <vector>
#include "catch.hpp"
#include "merlict/merlict.h"

using namespace merlict;
using std::vector;
using sensor::PhotonArrival;
using sensor::write_arrival_information_to_file;
using sensor::read_arrival_information_from_file;



TEST_CASE("SensorInOutTest: write_and_read", "[merlict]") {
    vector<PhotonArrival> arrivals_1;
    const unsigned int number_of_arrivals = 1337*42;
    random::Mt19937 prng(0);
    for (unsigned int i = 0; i < number_of_arrivals; i++) {
        PhotonArrival arrival;
        arrival.simulation_truth_id = prng.uniform();
        arrival.wavelength = prng.uniform();
        arrival.arrival_time = prng.uniform();
        arrival.x_intersect = prng.uniform();
        arrival.y_intersect = prng.uniform();
        arrival.theta_x = prng.uniform();
        arrival.theta_y = prng.uniform();
        arrivals_1.push_back(arrival);
    }
    CHECK(number_of_arrivals == arrivals_1.size());

    // write to disk
    std::ofstream out;
    out.open("InOut/arrival_information.bin", std::ios::out | std::ios::binary);
    write_arrival_information_to_file(&arrivals_1, &out);
    out.close();

    // read in again
    std::ifstream in;
    in.open("InOut/arrival_information.bin", std::ios::in | std::ios::binary);
    vector<PhotonArrival> arrivals_2 = read_arrival_information_from_file(
        &in,
        number_of_arrivals);
    in.close();

    REQUIRE(arrivals_2.size() == arrivals_1.size());
    for (unsigned int i = 0; i < number_of_arrivals; i++) {
        PhotonArrival ar1 = arrivals_1.at(i);
        PhotonArrival ar2 = arrivals_2.at(i);
        CHECK(ar1.simulation_truth_id == Approx(ar2.simulation_truth_id).margin(1e-5));
        CHECK(ar1.wavelength == Approx(ar2.wavelength).margin(1e-5));
        CHECK(ar1.arrival_time == Approx(ar2.arrival_time).margin(1e-5));
        CHECK(ar1.x_intersect == Approx(ar2.x_intersect).margin(1e-5));
        CHECK(ar1.y_intersect == Approx(ar2.y_intersect).margin(1e-5));
        CHECK(ar1.theta_x == Approx(ar2.theta_x).margin(1e-5));
        CHECK(ar1.theta_y == Approx(ar2.theta_y).margin(1e-5));
    }
}
