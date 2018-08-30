// Copyright 2017 Sebastian A. Mueller
#include <stdint.h>
#include "gtest/gtest.h"
#include "SignalProcessing/PhotonStream.h"
#include "SignalProcessing/ExtractedPulse.h"
#include "SignalProcessing/ElectricPulse.h"
#include "SignalProcessing/pulse_extraction.h"
#include "Core/Random/Random.h"
#include "Core/SimulationTruth.h"
using std::vector;
using std::string;

class PulseExtractionTest : public ::testing::Test {};

TEST_F(PulseExtractionTest, arrival_time_slices_below_next_channel_marker) {
    const float slice_duration = .5e-9;
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
        slice_duration,
        path);

    SignalProcessing::PhotonStream::Stream response_back =
        SignalProcessing::PhotonStream::read(path);

    EXPECT_EQ(response.size(), response_back.photon_stream.size());
}

TEST_F(PulseExtractionTest, truncate_invalid_arrival_times) {
    const float slice_duration = .5e-9;
    vector<vector<SignalProcessing::ElectricPulse>> response;
    vector<SignalProcessing::ElectricPulse> read_out_channel;
    for (int i = -1000; i < 1000; i++) {
        SignalProcessing::ElectricPulse pulse;
        pulse.arrival_time = slice_duration*i;
        pulse.simulation_truth_id = 0;
        read_out_channel.push_back(pulse);
    }
    response.push_back(read_out_channel);

    int number_invalid_photons = 0;
    for (uint32_t p = 0; p < response.at(0).size(); p++) {
        int32_t slice = round(
            response.at(0).at(p).arrival_time/slice_duration);
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
            slice_duration);

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
