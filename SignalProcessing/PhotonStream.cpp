// Copyright 2016 Sebastian A. Mueller
#include "SignalProcessing/PhotonStream.h"
#include <math.h>
#include <sstream>
#include <fstream>
using std::vector;
using std::string;

namespace SignalProcessing {
namespace PhotonStream {

void append_float32(const float &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

float read_float32(std::istream &fin) {
    float v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

void append_int32(const int32_t &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

int32_t read_int32(std::istream &fin) {
    int32_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

void append_uint32(const uint32_t &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

uint32_t read_uint32(std::istream &fin) {
    uint32_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

void append_uint8(const uint8_t &v, std::ostream &fout) {
    fout.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

uint8_t read_uint8(std::istream &fin) {
    uint8_t v;
    fin.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

Stream::Stream() {
    slice_duration = 0.0;
}

vector<vector<uint8_t>> truncate_arrival_times(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses,
    const float slice_duration
) {
    vector<vector<uint8_t>> channels;
    for (uint32_t ch = 0; ch < pulses.size(); ch++) {
        vector<uint8_t> channel;
        for (uint32_t pu = 0; pu < pulses.at(ch).size(); pu++) {
            int32_t slice = round(
                pulses.at(ch).at(pu).arrival_time/slice_duration);
            if (slice >= 0 && slice < NEXT_READOUT_CHANNEL_MARKER) {
                channel.push_back(
                    static_cast<uint8_t>(slice));
            }
        }
        channels.push_back(channel);
    }
    return channels;
}

void write(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses,
    const float slice_duration,
    const string path
) {
    vector<vector<uint8_t>> raw = truncate_arrival_times(
        pulses,
        slice_duration);
    const uint32_t number_channels = raw.size();
    const uint32_t number_symbols = number_of_symbols_to_represent_pulses(raw);
    const uint32_t number_time_slices = 100u;
    assert_number_time_slices_below_8bit_max(number_time_slices);

    std::ofstream file;
    file.open(path, std::ios::binary);

    if (!file.is_open()) {
        std::stringstream info;
        info << "Can not write file '" << path << "'.\n";
        throw std::runtime_error(info.str());
    }

    // PhotonStream Header 16 byte
    // -------------------
    append_float32(slice_duration, file);
    append_uint32(number_channels, file);
    append_uint32(number_time_slices, file);
    append_uint32(number_symbols, file);

    // Pulses
    // ------
    for (uint32_t ch = 0; ch < number_channels; ch++) {
        for (uint32_t pu = 0; pu < raw.at(ch).size(); pu++) {
            append_uint8(raw.at(ch).at(pu), file);
        }
        if (ch < number_channels-1)
            append_uint8(NEXT_READOUT_CHANNEL_MARKER, file);
    }

    file.close();
}

void write_simulation_truth(
    const vector<vector<SignalProcessing::ElectricPulse>> &pulses,
    const string path
) {
    std::ofstream file;
    file.open(path, std::ios::binary);

    if (!file.is_open()) {
        std::stringstream info;
        info << "Can not write file '" << path << "'.\n";
        throw std::runtime_error(info.str());
    }

    for (uint32_t ch = 0; ch < pulses.size(); ch++) {
        for (uint32_t pu = 0; pu < pulses.at(ch).size(); pu++) {
            append_int32(
                pulses.at(ch).at(pu).simulation_truth_id,
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

    stream.slice_duration = read_float32(file);
    read_uint32(file);
    uint32_t number_time_slices = read_uint32(file);
    assert_number_time_slices_below_8bit_max(number_time_slices);
    uint32_t number_symbols = read_uint32(file);

    if (number_symbols > 0) {
        vector<SignalProcessing::ElectricPulse> first_channel;
        stream.photon_stream.push_back(first_channel);
    }

    uint32_t channel = 0;
    for (uint32_t i = 0; i < number_symbols; i++) {
        unsigned char symbol = read_uint8(file);
        if (symbol == NEXT_READOUT_CHANNEL_MARKER) {
            channel++;
            if (i < number_symbols) {
                vector<SignalProcessing::ElectricPulse> next_channel;
                stream.photon_stream.push_back(next_channel);
            }
        } else {
            SignalProcessing::ElectricPulse pulse;
            pulse.arrival_time = symbol*stream.slice_duration;
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
                read_int32(file);
        }
    }

    file.close();
    return stream;
}

unsigned int number_of_pulses(
    const vector<vector<uint8_t>> &raw
) {
    unsigned int number_pulses = 0;
    for (unsigned int channel = 0; channel < raw.size(); channel++)
        number_pulses += raw.at(channel).size();
    return number_pulses;
}

unsigned int number_of_symbols_to_represent_pulses(
    const vector<vector<uint8_t>> &raw
) {
    uint32_t number_pulses_plus_number_channels =
        number_of_pulses(raw) + raw.size();

    uint32_t number_symbols;
    if (number_pulses_plus_number_channels > 0)
        number_symbols = number_pulses_plus_number_channels - 1;
    else
        number_symbols = 0;
    return number_symbols;
}

void assert_number_time_slices_below_8bit_max(
    const uint32_t number_time_slices
) {
    if (number_time_slices > NEXT_READOUT_CHANNEL_MARKER) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "Expected number_time_slices <= NEXT_READOUT_CHANNEL_MARKER ";
        info << "but actual number_time_slices = ";
        info << number_time_slices << ", ";
        info << "and NEXT_READOUT_CHANNEL_MARKER = ";
        info << NEXT_READOUT_CHANNEL_MARKER << ".\n";
        throw std::invalid_argument(info.str());
    }
}

}  // namespace PhotonStream
}  // namespace SignalProcessing
