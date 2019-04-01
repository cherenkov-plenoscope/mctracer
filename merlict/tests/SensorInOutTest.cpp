// Copyright 2014 Sebastian A. Mueller
#include <vector>
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("SensorInOutTest: write_and_read", "[merlict]") {
    std::vector<ml::sensor::PhotonArrival> arrivals_1;
    const unsigned int num_arrivals = 1337*42;
    ml::random::Mt19937 prng(0);
    for (unsigned int i = 0; i < num_arrivals; i++) {
        ml::sensor::PhotonArrival arrival;
        arrival.simulation_truth_id = prng.uniform();
        arrival.wavelength = prng.uniform();
        arrival.arrival_time = prng.uniform();
        arrival.x_intersect = prng.uniform();
        arrival.y_intersect = prng.uniform();
        arrival.theta_x = prng.uniform();
        arrival.theta_y = prng.uniform();
        arrivals_1.push_back(arrival);
    }
    CHECK(num_arrivals == arrivals_1.size());

    // write to disk
    std::ofstream out;
    out.open("merlict/tests/resources/arrival_information.bin", std::ios::out | std::ios::binary);
    ml::sensor::write_arrival_information_to_file(&arrivals_1, &out);
    out.close();

    // read in again
    std::ifstream in;
    in.open("merlict/tests/resources/arrival_information.bin", std::ios::in | std::ios::binary);
    std::vector<ml::sensor::PhotonArrival> arrivals_2 =
        ml::sensor::read_arrival_information_from_file(
            &in,
            num_arrivals);
    in.close();

    REQUIRE(arrivals_2.size() == arrivals_1.size());
    for (unsigned int i = 0; i < num_arrivals; i++) {
        ml::sensor::PhotonArrival ar1 = arrivals_1.at(i);
        ml::sensor::PhotonArrival ar2 = arrivals_2.at(i);
        CHECK(ar1.simulation_truth_id == Approx(ar2.simulation_truth_id).margin(1e-5));
        CHECK(ar1.wavelength == Approx(ar2.wavelength).margin(1e-5));
        CHECK(ar1.arrival_time == Approx(ar2.arrival_time).margin(1e-5));
        CHECK(ar1.x_intersect == Approx(ar2.x_intersect).margin(1e-5));
        CHECK(ar1.y_intersect == Approx(ar2.y_intersect).margin(1e-5));
        CHECK(ar1.theta_x == Approx(ar2.theta_x).margin(1e-5));
        CHECK(ar1.theta_y == Approx(ar2.theta_y).margin(1e-5));
    }
}
