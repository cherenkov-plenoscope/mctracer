#include "gtest/gtest.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Core/Random/Random.h"
#include <vector>
using std::vector;
using namespace PhotonSensor;

class SensorInOutTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(SensorInOutTest, write_and_read) {

    vector<PhotonArrival> arrivals_1;
    const unsigned int number_of_arrivals = 1337*42;

    Random::Mt19937 prng(0);

    for(unsigned int i=0; i<number_of_arrivals; i++) {
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

    EXPECT_EQ(arrivals_1.size(), number_of_arrivals);

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


    ASSERT_EQ(arrivals_1.size(), arrivals_2.size());
    for(unsigned int i=0; i<number_of_arrivals; i++) {

        PhotonArrival ar1 = arrivals_1.at(i);
        PhotonArrival ar2 = arrivals_2.at(i);
        EXPECT_NEAR(ar1.simulation_truth_id, ar2.simulation_truth_id, 1e-5);
        EXPECT_NEAR(ar1.wavelength, ar2.wavelength, 1e-5);
        EXPECT_NEAR(ar1.arrival_time, ar2.arrival_time, 1e-5);
        EXPECT_NEAR(ar1.x_intersect, ar2.x_intersect, 1e-5);
        EXPECT_NEAR(ar1.y_intersect, ar2.y_intersect, 1e-5);
        EXPECT_NEAR(ar1.theta_x, ar2.theta_x, 1e-5);
        EXPECT_NEAR(ar1.theta_y, ar2.theta_y, 1e-5);
    }
}