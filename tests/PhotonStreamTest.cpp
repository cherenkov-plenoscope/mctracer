// Copyright 2014 Sebastian A. Mueller
#include <stdint.h>
#include "gtest/gtest.h"
#include "signal_processing/signal_processing.h"
#include "Core/random/random.h"
#include "Core/SimulationTruth.h"
using std::vector;
using std::string;
using namespace signal_processing;
using namespace relleums;

vector<vector<ExtractedPulse>> create_photon_stream(
    const unsigned int number_of_channels,
    const float single_pulse_rate,
    const float exposure_time,
    const float time_slice_duration,
    const unsigned int seed) {

    random::Mt19937 prng(seed);

    vector<vector<ExtractedPulse>> photon_stream;
    for (unsigned int i = 0; i < number_of_channels; i++) {
        vector<ExtractedPulse> pulses_in_channel;
        float t = 0.0;
        while (true) {
            const float t_next = prng.expovariate(single_pulse_rate);
            if (t + t_next > exposure_time) {
                break;
            } else {
                t += t_next;
                ExtractedPulse pulse;
                pulse.arrival_time_slice = static_cast<int32_t>(
                    floor(t/time_slice_duration));
                pulse.simulation_truth_id = static_cast<int32_t>(
                    1000*prng.uniform());
                pulses_in_channel.push_back(pulse);
            }
        }
        photon_stream.push_back(pulses_in_channel);
    }

    return photon_stream;
}

void expect_eq(
    const PhotonStream::Stream &A,
    const PhotonStream::Stream &B,
    bool simulation_truth_eq = true
) {
    EXPECT_EQ(A.time_slice_duration, B.time_slice_duration);
    ASSERT_EQ(A.photon_stream.size(),B.photon_stream.size());
    for (unsigned int c = 0; c < A.photon_stream.size(); c++) {
        ASSERT_EQ(A.photon_stream.at(c).size(), B.photon_stream.at(c).size());
        for (unsigned int p = 0; p < A.photon_stream.at(c).size(); p++) {
            EXPECT_EQ(A.photon_stream.at(c).at(p).arrival_time_slice, B.photon_stream.at(c).at(p).arrival_time_slice);

            if (simulation_truth_eq) {
                EXPECT_EQ(A.photon_stream.at(c).at(p).simulation_truth_id, B.photon_stream.at(c).at(p).simulation_truth_id);
            }
        }
    }
}

void write_and_read_back(
    const unsigned int number_of_channels,
    const float time_slice_duration,
    const float single_pulse_rate,
    const float exposure_time,
    const unsigned int seed
) {
    PhotonStream::Stream stream;
    stream.photon_stream = create_photon_stream(
            number_of_channels,
            single_pulse_rate,
            exposure_time,
            time_slice_duration,
            seed);
    stream.time_slice_duration = time_slice_duration;

    const string path = "InOut/photon_stream.bin";
    PhotonStream::write(
        stream.photon_stream,
        time_slice_duration,
        path);

    const string truth_path = "InOut/photon_stream_truth.bin";
    PhotonStream::write_simulation_truth(
        stream.photon_stream,
        truth_path);

    PhotonStream::Stream ps_without_truth =
        PhotonStream::read(path);

    expect_eq(stream, ps_without_truth, false);

    PhotonStream::Stream ps_with_truth =
        PhotonStream::read_with_simulation_truth(
            path,
            truth_path);

    expect_eq(stream, ps_with_truth);
}

class PhotonStreamTest : public ::testing::Test {};

TEST_F(PhotonStreamTest, arrival_slices_must_not_be_NEXT_CHANNEL_MARKER) {
    vector<vector<ExtractedPulse>> channels;
    vector<ExtractedPulse> channel;
    const int32_t simulation_truth_id = 1337;
    channel.emplace_back(
        ExtractedPulse(
            PhotonStream::NEXT_CHANNEL_MARKER,
            simulation_truth_id));
    channels.push_back(channel);

    EXPECT_THROW(PhotonStream::write(channels, 0.5e-9, "InOut/must_not_be_written.phs"), std::runtime_error);

    channels.at(0).at(0).arrival_time_slice = 254;

    EXPECT_NO_THROW(PhotonStream::write(channels, 0.5e-9, "InOut/shall_be_written.phs"));
}

TEST_F(PhotonStreamTest, write_and_read_back_full_single_pulse_event) {
    const unsigned int number_of_channels = 1337;
    const float time_slice_duration = 0.5e-9;
    const float single_pulse_rate = 50e6;
    const float exposure_time = 127.0e-9;
    const unsigned int seed = 0;

    write_and_read_back(
        number_of_channels,
        time_slice_duration,
        single_pulse_rate,
        exposure_time,
        seed);
}

TEST_F(PhotonStreamTest, zero_channels) {
    const unsigned int number_of_channels = 0;
    const float time_slice_duration = 0.5e-9;
    const float single_pulse_rate = 50e6;
    const float exposure_time = 127.0e-9;
    const unsigned int seed = 0;

    write_and_read_back(
        number_of_channels,
        time_slice_duration,
        single_pulse_rate,
        exposure_time,
        seed);
}

TEST_F(PhotonStreamTest, empty_channels) {
    const unsigned int number_of_channels = 1337;
    const float time_slice_duration = 0.5e-9;
    const float single_pulse_rate = 0.0;
    const float exposure_time = 127.0e-9;
    const unsigned int seed = 0;

    write_and_read_back(
        number_of_channels,
        time_slice_duration,
        single_pulse_rate,
        exposure_time,
        seed);
}
