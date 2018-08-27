// Copyright 2016 Sebastian A. Mueller
#ifndef MCTRACER_SIGNALPROCESSING_PHOTONSTREAM_H_
#define MCTRACER_SIGNALPROCESSING_PHOTONSTREAM_H_

#include <vector>
#include <string>
#include "SignalProcessing/ElectricPulse.h"

namespace SignalProcessing {
namespace PhotonStream {

// Write and read ElectricPulses in the very compact photon_stream format.

const unsigned char NEXT_READOUT_CHANNEL_MARKER = 255;

void write(
    const std::vector<std::vector<SignalProcessing::ElectricPulse>> &pulses,
    const float slice_duration,
    const std::string path);

void write_simulation_truth(
    const std::vector<std::vector<SignalProcessing::ElectricPulse>> &pulses,
    const std::string path);

struct Stream {
    std::vector<std::vector<SignalProcessing::ElectricPulse>> photon_stream;
    float slice_duration;
    Stream();
};

Stream read(const std::string path);

Stream read_with_simulation_truth(
    const std::string path,
    const std::string truth_path);

unsigned int number_of_pulses(
    const std::vector<std::vector<SignalProcessing::ElectricPulse>> &pulses);

unsigned int number_of_symbols_to_represent_pulses(
    const std::vector<std::vector<SignalProcessing::ElectricPulse>> &pulses);

void assert_number_time_slices_below_8bit_max(
    const uint32_t number_time_slices);

std::vector<std::vector<uint8_t>> pulses_to_8bit_arrival_slices (
    const std::vector<std::vector<SignalProcessing::ElectricPulse>> &pulses,
    const float slice_duration);

}  // namespace PhotonStream
}  // namespace SignalProcessing

#endif  // MCTRACER_SIGNALPROCESSING_PHOTONSTREAM_H_
