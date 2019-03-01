// Copyright 2017 Sebastian A. Mueller
#include <stdint.h>
#include "catch.hpp"
#include "signal_processing/signal_processing.h"
#include "merlict/random/random.h"
#include "merlict/simulation_truth.h"
#include "merlict/numeric.h"
namespace ml = merlict;
using std::vector;
using std::string;


TEST_CASE("PulseExtractionTest: arrival_time_slices_below_next_channel_marker", "[merlict]") {
    const float time_slice_duration = .5e-9;
    vector<vector<signal_processing::ExtractedPulse>> response;
    vector<signal_processing::ExtractedPulse> read_out_channel;
    signal_processing::ExtractedPulse pulse;
    pulse.arrival_time_slice = 254u;
    pulse.simulation_truth_id = 0;
    read_out_channel.push_back(pulse);
    response.push_back(read_out_channel);

    const string path = "InOut/photon_stream.bin";
    signal_processing::PhotonStream::write(
        response,
        time_slice_duration,
        path);

    signal_processing::PhotonStream::Stream response_back =
        signal_processing::PhotonStream::read(path);

    CHECK(response_back.photon_stream.size() == response.size());
}

TEST_CASE("PulseExtractionTest: truncate_invalid_arrival_times", "[merlict]") {
    ml::random::Mt19937 prng(0);
    const double time_slice_duration = .5e-9;
    const double arrival_time_std = 0.0;

    vector<vector<signal_processing::ElectricPulse>> response;
    vector<signal_processing::ElectricPulse> read_out_channel;
    for (int i = -1000; i < 1000; i++) {
        signal_processing::ElectricPulse pulse;
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
            slice >= signal_processing::NUMBER_TIME_SLICES
        )
            number_invalid_photons++;
    }

    CHECK(2000 - signal_processing::NUMBER_TIME_SLICES == number_invalid_photons);

    vector<vector<signal_processing::ExtractedPulse>> raw =
        signal_processing::extract_pulses(
            response,
            time_slice_duration,
            arrival_time_std,
            &prng);

    REQUIRE(raw.size() == response.size());
    REQUIRE(1u == response.size());
    CHECK_FALSE(response.at(0).size() == raw.at(0).size());

    int number_of_passing_photons = 0;
    for (uint32_t ch = 0; ch < raw.size(); ch++) {
        for (uint32_t ph = 0; ph < raw.at(ch).size(); ph++) {
            number_of_passing_photons++;
            REQUIRE(255 > raw.at(ch).at(ph).arrival_time_slice);
        }
    }

    CHECK(signal_processing::NUMBER_TIME_SLICES == number_of_passing_photons);
}

TEST_CASE("PulseExtractionTest: arrival_time_std", "[merlict]") {
    ml::random::Mt19937 prng(0);
    const double time_slice_duration = .5e-9;
    const double arrival_time_std = 5e-9;
    const double true_arrival_time = 25e-9;

    vector<vector<signal_processing::ElectricPulse>> response;
    vector<signal_processing::ElectricPulse> read_out_channel;
    for (int i = 0; i < 10*1000; i++) {
        signal_processing::ElectricPulse pulse;
        pulse.arrival_time = true_arrival_time;
        pulse.simulation_truth_id = i;
        read_out_channel.push_back(pulse);
    }
    response.push_back(read_out_channel);

    vector<double> true_arrival_times;
    for (signal_processing::ElectricPulse &pulse : response.at(0))
        true_arrival_times.push_back(pulse.arrival_time);

    CHECK(0.0 == Approx(ml::numeric::stddev(true_arrival_times)).margin(1e-1));
    CHECK(true_arrival_time == Approx(ml::numeric::mean(true_arrival_times)).margin(1e-1));

    vector<vector<signal_processing::ExtractedPulse>> raw =
        signal_processing::extract_pulses(
            response,
            time_slice_duration,
            arrival_time_std,
            &prng);

    vector<double> reconstructed_arrival_times;
    for (signal_processing::ExtractedPulse &pulse : raw.at(0))
        reconstructed_arrival_times.push_back(
            pulse.arrival_time_slice * time_slice_duration);

    CHECK(arrival_time_std == Approx(ml::numeric::stddev(reconstructed_arrival_times)).margin(1e-10));
    CHECK(true_arrival_time == Approx(ml::numeric::mean(reconstructed_arrival_times)).margin(1e-10));
}
