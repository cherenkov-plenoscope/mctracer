// Copyright 2014 Sebastian A. Mueller
#include <stdint.h>
#include "tests/catch.hpp"
#include "signal_processing/signal_processing.h"
#include "merlict/random/random.h"
#include "merlict/simulation_truth.h"
namespace ml = merlict;
namespace sp = signal_processing;


std::vector<std::vector<sp::ExtractedPulse>> create_photon_stream(
    const unsigned int num_channels,
    const float single_pulse_rate,
    const float exposure_time,
    const float time_slice_duration,
    const unsigned int seed) {

    ml::random::Mt19937 prng(seed);

    std::vector<std::vector<sp::ExtractedPulse>> photon_stream;
    for (unsigned int i = 0; i < num_channels; i++) {
        std::vector<sp::ExtractedPulse> pulses_in_channel;
        float t = 0.0;
        while (true) {
            const float t_next = prng.expovariate(single_pulse_rate);
            if (t + t_next > exposure_time) {
                break;
            } else {
                t += t_next;
                sp::ExtractedPulse pulse;
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
    const sp::PhotonStream::Stream &A,
    const sp::PhotonStream::Stream &B,
    bool simulation_truth_eq = true
) {
    CHECK(B.time_slice_duration == A.time_slice_duration);
    REQUIRE(B.photon_stream.size() == A.photon_stream.size());
    for (unsigned int c = 0; c < A.photon_stream.size(); c++) {
        REQUIRE(B.photon_stream.at(c).size() == A.photon_stream.at(c).size());
        for (unsigned int p = 0; p < A.photon_stream.at(c).size(); p++) {
            CHECK(
                B.photon_stream.at(c).at(p).arrival_time_slice ==
                A.photon_stream.at(c).at(p).arrival_time_slice);

            if (simulation_truth_eq) {
                CHECK(
                    B.photon_stream.at(c).at(p).simulation_truth_id ==
                    A.photon_stream.at(c).at(p).simulation_truth_id);
            }
        }
    }
}

void write_and_read_back(
    const unsigned int num_channels,
    const float time_slice_duration,
    const float single_pulse_rate,
    const float exposure_time,
    const unsigned int seed
) {
    sp::PhotonStream::Stream stream;
    stream.photon_stream = create_photon_stream(
            num_channels,
            single_pulse_rate,
            exposure_time,
            time_slice_duration,
            seed);
    stream.time_slice_duration = time_slice_duration;

    const std::string path =
        "signal_processing/"
        "tests/"
        "resources/"
        "photon_stream.bin.tmp";
    sp::PhotonStream::write(
        stream.photon_stream,
        time_slice_duration,
        path);

    const std::string truth_path =
        "signal_processing/"
        "tests/"
        "resources/"
        "photon_stream_truth.bin.tmp";
    sp::PhotonStream::write_simulation_truth(
        stream.photon_stream,
        truth_path);

    sp::PhotonStream::Stream ps_without_truth =
        sp::PhotonStream::read(path);

    expect_eq(stream, ps_without_truth, false);

    sp::PhotonStream::Stream ps_with_truth =
        sp::PhotonStream::read_with_simulation_truth(
            path,
            truth_path);

    expect_eq(stream, ps_with_truth);
}



TEST_CASE("PhotonStreamTest: arrival_slices_must_not_be_NEXT_CHANNEL_MARKER", "[merlict]") {
    std::vector<std::vector<sp::ExtractedPulse>> channels;
    std::vector<sp::ExtractedPulse> channel;
    const int32_t simulation_truth_id = 1337;
    channel.emplace_back(
        sp::ExtractedPulse(
            sp::PhotonStream::NEXT_CHANNEL_MARKER,
            simulation_truth_id));
    channels.push_back(channel);

    CHECK_THROWS_AS(
        sp::PhotonStream::write(
            channels,
            0.5e-9,
            "signal_processing/"
            "tests/"
            "resources/"
            "must_throw_when_written.phs.tmp"),
        std::runtime_error);

    channels.at(0).at(0).arrival_time_slice = 254;

    CHECK_NOTHROW(
        sp::PhotonStream::write(
            channels,
            0.5e-9,
            "signal_processing/"
            "tests/"
            "resources/"
            "shall_not_throw_when_written.phs.tmp"));
}

TEST_CASE("PhotonStreamTest: write_and_read_back_full_single_pulse_event", "[merlict]") {
    const unsigned int num_channels = 1337;
    const float time_slice_duration = 0.5e-9;
    const float single_pulse_rate = 50e6;
    const float exposure_time = 127.0e-9;
    const unsigned int seed = 0;

    write_and_read_back(
        num_channels,
        time_slice_duration,
        single_pulse_rate,
        exposure_time,
        seed);
}

TEST_CASE("PhotonStreamTest: zero_channels", "[merlict]") {
    const unsigned int num_channels = 0;
    const float time_slice_duration = 0.5e-9;
    const float single_pulse_rate = 50e6;
    const float exposure_time = 127.0e-9;
    const unsigned int seed = 0;

    write_and_read_back(
        num_channels,
        time_slice_duration,
        single_pulse_rate,
        exposure_time,
        seed);
}

TEST_CASE("PhotonStreamTest: empty_channels", "[merlict]") {
    const unsigned int num_channels = 1337;
    const float time_slice_duration = 0.5e-9;
    const float single_pulse_rate = 0.0;
    const float exposure_time = 127.0e-9;
    const unsigned int seed = 0;

    write_and_read_back(
        num_channels,
        time_slice_duration,
        single_pulse_rate,
        exposure_time,
        seed);
}
