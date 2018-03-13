// Copyright 2016 Sebastian A. Mueller
#ifndef MCTRACER_SIGNALPROCESSING_PHOTONSTREAM_H_
#define MCTRACER_SIGNALPROCESSING_PHOTONSTREAM_H_

#include <vector>
#include <string>
#include "SignalProcessing/ElectricPulse.h"

namespace SignalProcessing {
namespace PhotonStream {

void append_float32(const float &v, std::ostream &fout);
float read_float32(std::istream &fin);
void append_int64(const int64_t &v, std::ostream &fout);
void append_uint32(const uint32_t &v, std::ostream &fout);
uint32_t read_uint32(std::istream &fin);
void append_uint8(const uint8_t &v, std::ostream &fout);
uint8_t read_uint8(std::istream &fin);

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

}  // namespace PhotonStream
}  // namespace SignalProcessing

#endif  // MCTRACER_SIGNALPROCESSING_PHOTONSTREAM_H_
