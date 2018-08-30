// Copyright 2017 Sebastian A. Mueller
#include <stdint.h>
#include "gtest/gtest.h"
#include "SignalProcessing/PhotonStream.h"
#include "SignalProcessing/ExtractedPulse.h"
#include "SignalProcessing/ElectricPulse.h"
#include "SignalProcessing/pulse_extraction.h"
#include "Core/Random/Random.h"
#include "Core/SimulationTruth.h"
#include "Tools/Numeric.h"
using std::vector;
using std::string;

class PulseExtractionTest : public ::testing::Test {};

TEST_F(PulseExtractionTest, arrival_time_slices_below_next_channel_marker) {
    const float time_slice_duration = .5e-9;
    vector<vector<SignalProcessing::ExtractedPulse>> response;
    vector<SignalProcessing::ExtractedPulse> read_out_channel;
    SignalProcessing::ExtractedPulse pulse;
    pulse.arrival_time_slice = 254u;
    pulse.simulation_truth_id = 0;
    read_out_channel.push_back(pulse);
    response.push_back(read_out_channel);

    const string path = "InOut/photon_stream.bin";
    SignalProcessing::PhotonStream::write(
        response,
        time_slice_duration,
        path);

    SignalProcessing::PhotonStream::Stream response_back =
        SignalProcessing::PhotonStream::read(path);

    EXPECT_EQ(response.size(), response_back.photon_stream.size());
}

TEST_F(PulseExtractionTest, truncate_invalid_arrival_times) {
    Random::Mt19937 prng(0);
    const double time_slice_duration = .5e-9;
    const double arrival_time_std = 0.0;

    vector<vector<SignalProcessing::ElectricPulse>> response;
    vector<SignalProcessing::ElectricPulse> read_out_channel;
    for (int i = -1000; i < 1000; i++) {
        SignalProcessing::ElectricPulse pulse;
        pulse.arrival_time = time_slice_duration*i;
        pulse.simulation_truth_id = 0;
        read_out_channel.push_back(pulse);
    }
    response.push_back(read_out_channel);

    int number_invalid_photons = 0;
    for (uint32_t p = 0; p < response.at(0).size(); p++) {
        int32_t slice = round(
            response.at(0).at(p).arrival_time/time_slice_duration);
        if (
            slice < 0 ||
            slice >= SignalProcessing::NUMBER_TIME_SLICES
        )
            number_invalid_photons++;
    }

    EXPECT_EQ(
        number_invalid_photons,
        2000 - SignalProcessing::NUMBER_TIME_SLICES);

    vector<vector<SignalProcessing::ExtractedPulse>> raw =
        SignalProcessing::extract_pulses(
            response,
            time_slice_duration,
            arrival_time_std,
            &prng);

    ASSERT_EQ(response.size(), raw.size());
    ASSERT_EQ(response.size(), 1u);
    EXPECT_FALSE(response.at(0).size() == raw.at(0).size());

    int number_of_passing_photons = 0;
    for (uint32_t ch = 0; ch < raw.size(); ch++) {
        for (uint32_t ph = 0; ph < raw.at(ch).size(); ph++) {
            number_of_passing_photons++;
            ASSERT_LT(raw.at(ch).at(ph).arrival_time_slice, 255);
        }
    }

    EXPECT_EQ(
        number_of_passing_photons,
        SignalProcessing::NUMBER_TIME_SLICES);
}

TEST_F(PulseExtractionTest, arrival_time_std) {
    Random::Mt19937 prng(0);
    const double time_slice_duration = .5e-9;
    const double arrival_time_std = 5e-9;
    const double true_arrival_time = 25e-9;

    vector<vector<SignalProcessing::ElectricPulse>> response;
    vector<SignalProcessing::ElectricPulse> read_out_channel;
    for (int i = 0; i < 10*1000; i++) {
        SignalProcessing::ElectricPulse pulse;
        pulse.arrival_time = true_arrival_time;
        pulse.simulation_truth_id = i;
        read_out_channel.push_back(pulse);
    }
    response.push_back(read_out_channel);

    vector<double> true_arrival_times;
    for (SignalProcessing::ElectricPulse &pulse : response.at(0))
        true_arrival_times.push_back(pulse.arrival_time);

    EXPECT_NEAR(0.0, Numeric::stddev(true_arrival_times), 1e-1);
    EXPECT_NEAR(true_arrival_time, Numeric::mean(true_arrival_times), 1e-1);

    vector<vector<SignalProcessing::ExtractedPulse>> raw =
        SignalProcessing::extract_pulses(
            response,
            time_slice_duration,
            arrival_time_std,
            &prng);

    vector<double> reconstructed_arrival_times;
    for (SignalProcessing::ExtractedPulse &pulse : raw.at(0))
        reconstructed_arrival_times.push_back(
            pulse.arrival_time_slice * time_slice_duration);

    EXPECT_NEAR(
        arrival_time_std,
        Numeric::stddev(reconstructed_arrival_times),
        1e-10);
    EXPECT_NEAR(
        true_arrival_time,
        Numeric::mean(reconstructed_arrival_times),
        1e-10);
}
