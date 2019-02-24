// Copyright 2016 Sebastian A. Mueller
#include "signal_processing/PhotonStream.h"
#include <math.h>
#include <sstream>
#include <fstream>
#include "signal_processing/pulse_extraction.h"
#include "Core/binary_input_output.h"
using std::vector;
using std::string;
using namespace relleums;

namespace signal_processing {
namespace PhotonStream {

Stream::Stream() {
    time_slice_duration = 0.0;
}

void write(
    const vector<vector<ExtractedPulse>> &channels,
    const float time_slice_duration,
    const string path
) {
    const uint32_t number_channels = channels.size();
    const uint32_t number_symbols = number_symbols_to_represent(channels);

    std::ofstream file;
    file.open(path, std::ios::binary);

    if (!file.is_open()) {
        std::stringstream info;
        info << "Can not write file '" << path << "'.\n";
        throw std::runtime_error(info.str());
    }

    // PhotonStream Header 16 byte
    // -------------------
    bio::append_float32(time_slice_duration, file);
    bio::append_uint32(number_channels, file);
    bio::append_uint32(NUMBER_TIME_SLICES, file);
    bio::append_uint32(number_symbols, file);

    // Pulses
    // ------
    for (uint64_t ch = 0; ch < number_channels; ch++) {
        for (uint64_t pu = 0; pu < channels.at(ch).size(); pu++) {
            if (
                channels.at(ch).at(pu).arrival_time_slice ==
                NEXT_CHANNEL_MARKER
            ) {
                std::stringstream info;
                info << "PhotonStream::write(" << path << ")\n";
                info << "Expected arrival slice of photon != ";
                info << "NEXT_CHANNEL_MARKER\n";
                throw std::runtime_error(info.str());
            }
            bio::append_uint8(channels.at(ch).at(pu).arrival_time_slice, file);
        }
        if (ch < number_channels-1)
            bio::append_uint8(NEXT_CHANNEL_MARKER, file);
    }

    file.close();
}

void write_simulation_truth(
    const vector<vector<ExtractedPulse>> &channels,
    const string path
) {
    std::ofstream file;
    file.open(path, std::ios::binary);

    if (!file.is_open()) {
        std::stringstream info;
        info << "Can not write file '" << path << "'.\n";
        throw std::runtime_error(info.str());
    }

    for (uint32_t ch = 0; ch < channels.size(); ch++) {
        for (uint32_t pu = 0; pu < channels.at(ch).size(); pu++) {
            bio::append_int32(
                channels.at(ch).at(pu).simulation_truth_id,
                file);
        }
    }

    file.close();
}

Stream read(const string path) {
    Stream stream;

    std::ifstream file;
    file.open(path, std::ios::binary);

    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "PhotonStream: Unable to open file: '" << path << "'\n";
        throw std::runtime_error(info.str());}

    stream.time_slice_duration = bio::read_float32(file);
    bio::read_uint32(file);
    uint32_t number_time_slices = bio::read_uint32(file);
    if (number_time_slices != NUMBER_TIME_SLICES) {
        std::stringstream info;
        info << "PhotonStream::read(" << path << ")\n";
        info << "Expected number_time_slices == " << NUMBER_TIME_SLICES;
        info << ", but actual it is " << number_time_slices << "\n";
        throw std::runtime_error(info.str());
    }
    uint32_t number_symbols = bio::read_uint32(file);

    if (number_symbols > 0) {
        vector<ExtractedPulse> first_channel;
        stream.photon_stream.push_back(first_channel);
    }

    uint32_t channel = 0;
    for (uint32_t i = 0; i < number_symbols; i++) {
        uint8_t symbol = bio::read_uint8(file);
        if (symbol == NEXT_CHANNEL_MARKER) {
            channel++;
            if (i < number_symbols) {
                vector<ExtractedPulse> next_channel;
                stream.photon_stream.push_back(next_channel);
            }
        } else {
            ExtractedPulse pulse;
            pulse.arrival_time_slice = symbol;
            stream.photon_stream.at(channel).push_back(pulse);
        }
    }

    file.close();

    return stream;
}

Stream read_with_simulation_truth(const string path, const string truth_path) {
    Stream stream = read(path);

    std::ifstream file;
    file.open(truth_path, std::ios::binary);
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "PhotonStream: Unable to open file: '" << truth_path << "'\n";
        throw std::runtime_error(info.str());}

    for (
        uint32_t channel = 0;
        channel < stream.photon_stream.size();
        channel++
    ) {
        for (
            uint32_t pulse = 0;
            pulse < stream.photon_stream.at(channel).size();
            pulse++
        ) {
            stream.photon_stream.at(channel).at(pulse).simulation_truth_id =
                bio::read_int32(file);
        }
    }

    file.close();
    return stream;
}

uint64_t number_pulses(
    const vector<vector<ExtractedPulse>> &raw
) {
    uint64_t number_pulses = 0;
    for (uint64_t channel = 0; channel < raw.size(); channel++)
        number_pulses += raw.at(channel).size();
    return number_pulses;
}

uint64_t number_symbols_to_represent(
    const vector<vector<ExtractedPulse>> &raw
) {
    uint64_t number_pulses_plus_number_channels =
        number_pulses(raw) + raw.size();

    uint64_t number_symbols;
    if (number_pulses_plus_number_channels > 0)
        number_symbols = number_pulses_plus_number_channels - 1;
    else
        number_symbols = 0;
    return number_symbols;
}

}  // namespace PhotonStream
}  // namespace signal_processing
