// Copyright 2016 Sebastian A. Mueller
#ifndef SIGNALPROCESSING_PHOTONSTREAM_H_
#define SIGNALPROCESSING_PHOTONSTREAM_H_

#include <stdint.h>
#include <vector>
#include <string>
#include "signal_processing/ExtractedPulse.h"

namespace signal_processing {
namespace PhotonStream {

// Write and read ElectricPulses in the very compact photon_stream format.

const uint8_t NEXT_CHANNEL_MARKER = 255;

void write(
    const std::vector<std::vector<ExtractedPulse>> &pulses,
    const float time_slice_duration,
    const std::string path);

void write_simulation_truth(
    const std::vector<std::vector<ExtractedPulse>> &pulses,
    const std::string path);

struct Stream {
    std::vector<std::vector<ExtractedPulse>> photon_stream;
    float time_slice_duration;
    Stream();
};

Stream read(const std::string path);

Stream read_with_simulation_truth(
    const std::string path,
    const std::string truth_path);

uint64_t num_pulses(
    const std::vector<std::vector<ExtractedPulse>> &pulses);

uint64_t num_symbols_to_represent(
    const std::vector<std::vector<ExtractedPulse>> &pulses);

}  // namespace PhotonStream
}  // namespace signal_processing

#endif  // SIGNALPROCESSING_PHOTONSTREAM_H_
